
#include <ctime>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QTimer>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>
#include <QtNetwork/QSslConfiguration>
#include <QtNetwork/QUdpSocket>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QIODevice>
#include <QFile>

#include "controler/deviceMonitor.h"
#include "testMonitor.h"


QT_USE_NAMESPACE

TestMonitor::TestMonitor(QObject *parent):DeviceMonitor(parent)
{
    connect(this, SIGNAL(deviceConnected(DeviceAgent*)), this, SLOT(onDeviceConnected(DeviceAgent*)),Qt::QueuedConnection);
    connect(this, SIGNAL(deviceDisconnected(DeviceAgent*)), this, SLOT(onDeviceDisconnected(DeviceAgent*)),Qt::QueuedConnection);
    connect(this, SIGNAL(dataReceived(QString &,int,QByteArray*)), parent, SLOT( onDataReceived(QString &,int,QByteArray*) )/*, Qt::QueuedConnection*/);

    QTimer::singleShot(1000,this,SLOT(onStartTest()));
}


int TestMonitor::onStartTest()
{
    srand(time(0));

    DeviceAgent *deviceSock = new DeviceAgent(this);
    deviceSock->m_devID = QString("test123%1").arg(rand());
    emit deviceConnected(deviceSock);
    
    return PL_RET_OK;
    
}

int TestMonitor::pimTest(QString &devID,uint32_t action,QByteArray *data)
{
    QJsonObject cmd;
    QJsonDocument sd = QJsonDocument::fromJson(*data);
    cmd = sd.object();
    int pim_action = cmd.value("pim_action").toInt();
    QString jsonFile = "";
    switch (pim_action)
    {
    case PIM_ACTION_GET_CONTACT_LIST:
    {
        jsonFile = "contact.json";
        break;
    }
    case PIM_ACTION_GET_SMS_LIST:
    {
        jsonFile = "sms.json";
        break;
    }
    case PIM_ACTION_GET_CALL_LIST:
    {
        jsonFile = "call.json";
        break;
    }
    case PIM_ACTION_GET_CONTACT_DATA_LIST:
    {
        jsonFile = "contact_data.json";
        break;
    }

    }

    if (jsonFile.length() < 1)
    {
        DEBUG_INFO << "Can not test pim_action" + pim_action;
        return PL_RET_FAIL;
    }

    QJsonObject jsonObj;
    if (PL_RET_OK != loadJsonObject(jsonFile,jsonObj))
    {
        DEBUG_INFO << "Can not load json data:" + jsonFile;
        return PL_RET_FAIL;
    }

    QJsonDocument doc(jsonObj);
    QByteArray d1 = doc.toJson(QJsonDocument::Compact);
    if (d1.length() < 1)
    {
        DEBUG_INFO << "invalid para for get sys info";
        return PL_RET_FAIL;
    }

    QByteArray *d2 = new QByteArray(d1);
    emit dataReceived(devID,action,d2);

    return PL_RET_OK;
}



int TestMonitor::generalTest(QString &devID,uint32_t action,QByteArray *data)
{

    QString fileName = "";

    switch (action)
    {
    case PL_REQ_GET_SYS_INFO:
    {
        if (data != NULL)
        {
            DEBUG_INFO << "invalid para for get sys info";
            return PL_RET_FAIL;
        }

        fileName = "sys.json";

        break;
    }
    case  PL_REQ_GET_APP_LIST:
    {
        fileName = "applist.json";
        break;
    }
    case  PL_REQ_GET_APP_ICON:
    {
        break;
    }
    case  PL_REQ_DEL_APP:
    {
        break;
    }
    case  PL_REQ_INSTALL_APP:
    {
        break;
    }
    case  PL_REQ_GET_SD_CARD_LIST:
    {
        break;
    }
    case  PL_REQ_GET_FILE_LIST:
    {
        if (data == NULL)
        {
            DEBUG_INFO << "invalid para for get sys info";
            return PL_RET_FAIL;
        }
        fileName = "filelist.json";
        break;
    }
    }

    QJsonObject sys;
    if (PL_RET_OK != loadJsonObject(fileName,sys))
    {
        DEBUG_INFO << "Load json test data failed";
        return PL_RET_FAIL;
    }
	
    QJsonDocument doc;
    doc.setObject(sys);

    QByteArray d1 = doc.toJson(QJsonDocument::Compact);
    if (d1.length() < 1)
    {
        DEBUG_INFO << "invalid para action:" + action;
        return PL_RET_FAIL;
    }

    QByteArray *d2 = new QByteArray(d1);//memory leak

    emit dataReceived(devID,action,d2);

}


int TestMonitor::sendCmd(QString &devID,uint32_t action,QByteArray *data)
{
    if (!devID.contains("test123"))
    {
        DEBUG_INFO << "invalid dev id found";
        return PL_RET_FAIL;
    }

    if (data != NULL)
    {
        if ( !isJsonArray(*data) && !isJsonObject(*data))
        {
            DEBUG_INFO << "invalid command data,must be json format";
            return PL_RET_FAIL;
        }
    }

    if (action == PL_REQ_GET_PIM_DATA)
    {
        return pimTest(devID, action, data);
    }

    return generalTest(devID, action, data);
}


int TestMonitor::isJsonObject(QByteArray data)
{
    QJsonDocument sd = QJsonDocument::fromJson(data);
    return sd.isObject();
}


int TestMonitor::isJsonArray(QByteArray data)
{
    QJsonDocument sd = QJsonDocument::fromJson(data);
    return sd.isArray();

}


int TestMonitor::loadJsonObject(QString &fileName,QJsonObject &jsonObj )
{
    pthread_mutex_lock(&conn_mutex);
    QFile file;

    QString sampleDir = "../../doc/json_sample/";
    sampleDir = "d:\\phonelin\\doc\\json_sample\\";
#if __linux__
    sampleDir = "/home/hamigua/phonelin/doc/json_sample/";
#endif

    file.setFileName(sampleDir + fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        DEBUG_INFO << "Failed to open json sameple file:" + sampleDir + fileName;
        pthread_mutex_unlock(&conn_mutex);
        return PL_RET_FAIL;
    }

    QString data = file.readAll();
    file.close();

    QJsonDocument sd = QJsonDocument::fromJson(data.toUtf8());
    if (!sd.isObject() || sd.isNull() || sd.isEmpty())
    {
        DEBUG_INFO << "Failed to open json sameple file:" + fileName;
        pthread_mutex_unlock(&conn_mutex);
        return PL_RET_FAIL;
    }

    jsonObj = sd.object();

    pthread_mutex_unlock(&conn_mutex);

    return PL_RET_OK;

}



int TestMonitor::loadJsonArray(QString &fileName,QJsonArray &jsonAry )
{
    pthread_mutex_lock(&conn_mutex);
    QFile file;

    QString sampleDir = "doc/json_sample/";
    file.setFileName(sampleDir + fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        DEBUG_INFO << "Failed to open json sameple file:" + fileName;
        return PL_RET_FAIL;
    }

    QString data = file.readAll();
    file.close();

    QJsonDocument sd = QJsonDocument::fromJson(data.toUtf8());
    if (!sd.isArray() || sd.isNull() || sd.isEmpty())
    {
        DEBUG_INFO << "Failed to open json sameple file:" + fileName;
        pthread_mutex_unlock(&conn_mutex);
        return PL_RET_FAIL;
    }

    jsonAry = sd.array();

    return PL_RET_OK;

}






