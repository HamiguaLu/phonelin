
#include <QFile>
#include <QJsonDocument>
#include <QJsonValue>
#include <QEventLoop>
#include <QApplication>
#include "deviceAgent.h"


#ifdef Q_OS_WIN32

#pragma comment(lib, "Ws2_32.lib")
#else
#include <arpa/inet.h>
#endif

#include "jsonHelper.h"

union MyUnion {
   int64_t i64;
   int32_t i32[2];
};

int64_t htonl64(int64_t hostFormatInt64)
{
   MyUnion u;
   u.i64 = hostFormatInt64;
   int32_t temp = u.i32[0];
   u.i32[0] = htonl(u.i32[1]);
   u.i32[1] = htonl(temp);
   return u.i64;
}

int64_t ntohl64(int64_t networkFormatInt64)
{
   MyUnion u;
   u.i64 = networkFormatInt64;
   int32_t temp = u.i32[0];
   u.i32[0] = ntohl(u.i32[1]);
   u.i32[1] = ntohl(temp);
   return u.i64;
}


DeviceAgent::DeviceAgent(QObject *parent): QSslSocket(parent)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(onDataComing()));
    connect(this, SIGNAL(disconnected()), this, SLOT(onDeviceDisconnected()));

    connect(this, SIGNAL(encrypted()), this, SLOT(onEncrypted()));
    connect(this, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(onSslErrors(QList<QSslError>)));

    m_dataPos = 0;
}

DeviceAgent::~DeviceAgent()
{
    DEBUG_INFO << "DeviceAgent destructor";
}



int DeviceAgent::sendFile(uint32_t action,QString &localFileName,QString &dstFileName)
{
    pthread_mutex_lock(&conn_mutex);

    if (dstFileName.isEmpty() || localFileName.isEmpty())
    {
        DEBUG_INFO << "fileName is invalid";
        pthread_mutex_unlock(&conn_mutex);
        return PL_RET_FAIL;
    }


    QFile file;
    file.setFileName(localFileName);


    if (!file.open(QIODevice::ReadOnly) || file.size() < 1)
    {
        DEBUG_INFO << "Failed to open file:" + localFileName;
        pthread_mutex_unlock(&conn_mutex);
        return PL_RET_FAIL;
    }

    QJsonObject cmd;
    cmd.insert("path",localFileName);
    cmd.insert("dstPath",dstFileName);

    QJsonDocument doc(cmd);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact);

    uint32_t len = jsonData.size();

    NetPackageHdr hdr;
    hdr.TAG = htonl(action);
    hdr.data_len = htonl64(file.size() + len);

    if (sizeof(NetPackageHdr)  != write( (char *)&hdr,sizeof(NetPackageHdr)))\
    {
        file.close();
        pthread_mutex_unlock(&conn_mutex);
        return PL_RET_FAIL;
    }

    //flush();

    uint32_t nlen = htonl(len);
    if (sizeof(nlen) != write((char*)&nlen,sizeof(nlen)))
    {
        file.close();
        pthread_mutex_unlock(&conn_mutex);
        return PL_RET_FAIL;
    }

    if (len != write(jsonData))
    {
        file.close();
        pthread_mutex_unlock(&conn_mutex);
        return PL_RET_FAIL;
    }

    qint64 fLen = file.size();
    qint64 fTotal = fLen;
    int iRead = 0;
    char buf[10240];
    emit busyMessage("Sendd file...");
    while (fLen > 0 && (iRead = file.read(buf,10240)) > 0)
    {
        if (iRead != write(buf,iRead))
        {
            file.close();
            pthread_mutex_unlock(&conn_mutex);
            return PL_RET_FAIL;
        }

        fLen -= iRead;
        if (++m_iLoopCounter % 100 == 0)
        {
            //flush();
            QApplication::processEvents();
            QString info = QString("Sending file %1% left").arg((fLen * 100)/fTotal);
            emit busyMessage(info);
        }
    }

    //flush();

    emit busyMessage("Send file done,wait response from device...");

    file.close();
    if (fLen != 0)
    {
        pthread_mutex_unlock(&conn_mutex);
        return PL_RET_FAIL;
    }

    pthread_mutex_unlock(&conn_mutex);
    return PL_RET_OK;
}



int DeviceAgent::sendData(uint32_t action,QByteArray *data)
{
    pthread_mutex_lock(&conn_mutex);
    DEBUG_INFO << "Enter";
    NetPackageHdr hdr;
    hdr.TAG = htonl(action);
    if (data != NULL && data->length() > 0)
    {
        hdr.data_len = htonl64(data->length());
    }
    else
    {
        hdr.data_len = 0;
    }

    if (sizeof(NetPackageHdr)  != write( (char *)&hdr,sizeof(NetPackageHdr)))\
    {
        pthread_mutex_unlock(&conn_mutex);
        return PL_RET_FAIL;
    }

    if (data == NULL || data->length() == 0)
    {
        //flush();
        pthread_mutex_unlock(&conn_mutex);
        return PL_RET_OK;
    }

    if (data->length()  != write(*data))
    {
        pthread_mutex_unlock(&conn_mutex);
        return PL_RET_FAIL;
    }

    //flush();
    pthread_mutex_unlock(&conn_mutex);

    return PL_RET_OK;
}

void DeviceAgent::onDataComing()
{
START_RECV:
    if (m_dataPos == 0 && m_readState == READ_STATE_WAIT_PACKAGE_HEADER)
    {
        memset(&m_dataHdr, 0 ,sizeof(m_dataHdr));
        m_datalen = sizeof(m_dataHdr);

        delete m_data;
        m_data = NULL;
    }

    if (m_readState == READ_STATE_WAIT_PACKAGE_HEADER)
    {
        char *hdrStart = (char*)&m_dataHdr + m_dataPos;
        int dataReaded = read(hdrStart,m_datalen - m_dataPos);
        if (dataReaded == 0)
        {
            //no data, wait for next read
            return;
        }

        m_dataPos += dataReaded;
        if (m_dataPos < sizeof(NetPackageHdr))
        {
            return;
        }

        if (m_dataPos > sizeof(NetPackageHdr))
        {
            DEBUG_INFO << "Read error!!!!!!!!!!";
			onPackageDataError();
            return;
        }

        m_dataHdr.TAG = ntohl(m_dataHdr.TAG);
        m_dataHdr.data_len = ntohl64(m_dataHdr.data_len);

        if (m_dataHdr.TAG == PL_REQ_DOWNLOAD_FILE)
        {
            m_readState = READ_STATE_WAIT_FILE_JSON_HDR_LEN;
            m_dataPos = 0;
            m_datalen = sizeof(qint64);
        }
        else
        {
            m_readState = READ_STATE_WAIT_PACKAGE_DATA;
            m_dataPos = 0;
            m_datalen = m_dataHdr.data_len;
        }

        if (m_datalen > 0)
        {
            m_data = new QByteArray(m_datalen,0);
        }

    }

    if (m_readState == READ_STATE_WAIT_PACKAGE_DATA)
    {
        if (m_datalen  > 0)
        {
            int dataReaded = read(m_data->data() + m_dataPos,m_datalen - m_dataPos);
            if (dataReaded == 0)
            {
                return;
            }

            m_dataPos += dataReaded;
        }

        if (m_dataPos < m_datalen)
        {
            return;
        }

        if (m_dataPos > m_datalen)
        {
            DEBUG_INFO << "Read error!!!!!!!!!!";
            DEBUG_INFO << "m_datalen:" <<m_datalen << " m_dataPos:" << m_dataPos;
			onPackageDataError();
            return;
        }


        m_readState = READ_STATE_WAIT_PACKAGE_HEADER;
        m_dataPos = 0;

        /*Notify controller here,need to block until data is copied out*/
        if (m_dataHdr.TAG == PL_REQ_REPORT_ID)
        {
            m_devID = m_data->data();
            emit deviceConnected(this);
        }
        else
        {
            emit dataReceived(m_devID,m_dataHdr.TAG,m_data);
        }

    }

    if (m_readState == READ_STATE_WAIT_FILE_JSON_HDR_LEN)
    {
        int dataReaded = read(m_data->data() + m_dataPos,m_datalen - m_dataPos);
        if (dataReaded == 0)
        {
            return;
        }

        m_dataPos += dataReaded;
        if (m_dataPos < m_datalen)
        {
            return;
        }

        if (m_dataPos > m_datalen)
        {
            DEBUG_INFO << "Read error!!!!!!!!!!";
			onPackageDataError();
            return;
        }

        m_readState = READ_STATE_WAIT_FILE_JSON_HDR;
        m_dataPos = 0;
        memcpy((char*)&m_datalen,m_data->data(),sizeof(m_datalen));
        m_datalen = ntohl64(m_datalen);

        delete m_data;
        m_data = NULL;
        if (m_datalen > 0)
        {
            m_data = new QByteArray(m_datalen,0);
        }
        else
        {
            onPackageDataError();
            return;
        }
    }

    if (m_readState == READ_STATE_WAIT_FILE_JSON_HDR)
    {
        int dataReaded = read(m_data->data() + m_dataPos,m_datalen - m_dataPos);
        if (dataReaded == 0)
        {
            return;
        }

        m_dataPos += dataReaded;
        if (m_dataPos < m_datalen)
        {
            return;
        }

        if (m_dataPos > m_datalen)
        {
            DEBUG_INFO << "Read error!!!!!!!!!!";
			onPackageDataError();
            return;
        }


        JsonHelper jsonHelper;
        if (PL_RET_OK != jsonHelper.parseObject(m_data))
        {
            onPackageDataError();
            return;
        }

        QJsonObject jsonHdr = jsonHelper.m_jsonObj;
        m_srcFileName = jsonHdr.value("path").toString();
        m_dstFileName = jsonHdr.value("dstPath").toString();
        m_fileTransferRetCode = jsonHdr.value("retCode").toInt();

        m_datalen = m_dataHdr.data_len - m_datalen;
        if (m_datalen == 0)
        {
            //need post error here
            DEBUG_INFO << "No file data";
            emit dataReceived(m_devID,m_dataHdr.TAG,NULL);
            m_dataPos = 0;
            m_readState = READ_STATE_WAIT_PACKAGE_HEADER;
        }
        else
        {
            m_readState = READ_STATE_WAIT_FILE_DATA;
            m_dataPos = 0;
        }

    }

    if (m_readState == READ_STATE_WAIT_FILE_DATA)
    {
        int buflen = 20480;
        char dataBuf[20480];
        if ((m_datalen - m_dataPos) < buflen)
        {
            //this is to make sure do not read extra data
            buflen = m_datalen - m_dataPos;

        }

        int dataReaded = read(dataBuf,buflen);
        if (dataReaded < 1)
        {
            return;
        }

        //save data to file
        m_downloadfile.setFileName(m_dstFileName);
        bool opened = false;
        if (m_dataPos == 0 )
        {
            //DEBUG_INFO << "Empty existing file" << m_dstFileName;
            opened = m_downloadfile.open(QIODevice::WriteOnly | QIODevice::Truncate);
        }
        else
        {
            opened = m_downloadfile.open( QIODevice::Append);
        }

        if (!opened)
        {
            DEBUG_INFO << "Failed to open file:" + m_dstFileName << m_downloadfile.error();
            m_downloadfile.remove();
            onPackageDataError();
            return;
        }


        if (m_downloadfile.write(dataBuf,dataReaded) != dataReaded)
        {
            DEBUG_INFO << "Failed to save file:" + m_downloadfile.error() << ":" <<m_downloadfile.errorString();
            m_downloadfile.close();
            m_downloadfile.remove();
            onPackageDataError();
            return;
        }

        m_downloadfile.close();

        m_dataPos += dataReaded;

        if (++m_iLoopCounter % 100 == 0 && m_datalen)
        {
            static qint64 last_percent = 0;
            qint64 percent = (m_dataPos*100)/m_datalen;
            if (percent != last_percent)
            {
                last_percent = percent;
                QString info = QString("%1% data received").arg(percent);
                emit busyMessage(info);
            }
        }

        if (m_dataPos == m_datalen)
        {
            emit dataReceived(m_devID,m_dataHdr.TAG,m_data);
            emit busyMessage("Receive file data done");
            m_dataPos = 0;
            m_readState = READ_STATE_WAIT_PACKAGE_HEADER;
        }
    }


    goto START_RECV;

}

void DeviceAgent::onPackageDataError()
{
    DEBUG_INFO << "Wrong data,Close connection";
    close();
}


void DeviceAgent::onDeviceDisconnected()
{
    DEBUG_INFO << "Device disconnected";
    emit deviceDisconnected(this);
    //delete this;
}


void DeviceAgent::onEncrypted()
{
    DEBUG_INFO << "Client onEncrypted:" << peerName();
}

void DeviceAgent::onSslErrors(const QList<QSslError> &)
{
    DEBUG_INFO << "Ssl errors occurred";
}



