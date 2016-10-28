
#include <QtCore/QDebug>
#include <QtCore/QFile>

#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>
#include <QtNetwork/QSslConfiguration>
#include <QtNetwork/QUdpSocket>
#include "deviceMonitor.h"
#include <QSettings>
#include <QNetworkAddressEntry>
#include <QNetworkInterface>
#include <QApplication>
#include <QDir>

#include "jsonHelper.h"
#include "pimProxy.h"
#include "appProxy.h"
#include "storageProxy.h"
#include "db/dbHelper.h"

#ifdef Q_OS_WIN32
#include<WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")
#endif

QT_USE_NAMESPACE



DeviceMonitor::DeviceMonitor(QObject *parent) : QTcpServer(parent),  m_clients()
{
    QSettings settings("PhoneLinSettings.ini", QSettings::IniFormat);
    m_sMediaSyncFolder = settings.value("mediaSyncFolder", "").toString();
    if (m_sMediaSyncFolder.isEmpty())
    {
        m_bAutoSyncMedia = false;
    }
    else
    {
        m_bAutoSyncMedia = settings.value("autoSyncMedia", "").toBool();
    }

    if (m_sMediaSyncFolder.isEmpty())
    {
        DEBUG_INFO << "Media folder is empty";
    }

    connect(this, SIGNAL(loadDevicePimData(QString)), this, SLOT(onLoadDevicePimData(QString)),Qt::QueuedConnection);
    connect(this, SIGNAL(getContactData(QString,int)), this, SLOT(onGetContactData(QString,int)),Qt::QueuedConnection);
    connect(this, SIGNAL(getSysInfo(QString)), this, SLOT(onGetSysInfo(QString)),Qt::QueuedConnection);
    connect(this, SIGNAL(installApp(QString,QString)), this, SLOT(onInstallApp(QString,QString)),Qt::QueuedConnection);
    connect(this, SIGNAL(getAppIcon(QString,QString)), this, SLOT(onGetAppIcon(QString,QString)),Qt::QueuedConnection);
    connect(this, SIGNAL(rmApp(QString,QString)), this, SLOT(onRmApp(QString,QString)),Qt::QueuedConnection);
    connect(this, SIGNAL(refreshAppList(QString,int)), this, SLOT( onRefreshAppList(QString,int)),Qt::QueuedConnection);
    connect(this, SIGNAL(loadMediaData(QString)), this, SLOT(onLoadMediaData(QString)),Qt::QueuedConnection);
    connect(this, SIGNAL(loadSDCardList(QString)), this, SLOT(onLoadSDCardList(QString)),Qt::QueuedConnection);
    connect(this, SIGNAL(installApp(QString,QString)), this, SLOT(onInstallApp(QString,QString)),Qt::QueuedConnection);
    connect(this, SIGNAL(getFileList(QString,QString,int)), this, SLOT(onGetFileList(QString,QString,int)),Qt::QueuedConnection);
    connect(this, SIGNAL(newFile(QString,QString,bool)), this, SLOT(onNewFile(QString,QString,bool)),Qt::QueuedConnection);
    connect(this, SIGNAL(renameFile(QString,QString,QString)), this, SLOT(onRenameFile(QString,QString,QString)),Qt::QueuedConnection);
    connect(this, SIGNAL(downloadFile(QString,QString,QString)), this, SLOT(onDownloadFile(QString,QString,QString)),Qt::QueuedConnection);
    connect(this, SIGNAL(uploadFile(QString,QString,QString)), this, SLOT(onUploadFile(QString,QString,QString)),Qt::QueuedConnection);
    connect(this, SIGNAL(removeFile(QString,QString)), this, SLOT(onRemoveFile(QString,QString)),Qt::QueuedConnection);
    connect(this, SIGNAL(copyFile(QString,int, QString,QString)), this, SLOT(onCopyFile(QString,int, QString,QString)),Qt::QueuedConnection);
    connect(this, SIGNAL(sendSMS(QString,QString,QString)), this, SLOT(onSendSMS(QString,QString,QString)),Qt::QueuedConnection);
}


void DeviceMonitor::start()
{
    initSSLData();
    startServer();

    initBroadcastAddr();

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(broadcastInfo()));
    m_timer->start(1000 * 10);
}


DeviceMonitor::~DeviceMonitor()
{
    //qDeleteAll(m_clients.begin(), m_clients.end());
    m_timer->stop();

    close();

    for (QMap<QString, DeviceAgent*>::iterator it = m_clients.begin(); it != m_clients.end();++it)
    {
        DeviceAgent* pDevice = reinterpret_cast<DeviceAgent *>(it.value());
        pDevice->close();
    }

}

int DeviceMonitor::startServer()
{
    if (listen(QHostAddress::Any, m_iPort))
    {
        return PL_RET_OK;
    }

    return PL_RET_FAIL;
}


void DeviceMonitor::initBroadcastAddr()
{
    QSettings settings("PhoneLinSettings.ini", QSettings::IniFormat);
    QString defIp = settings.value("defIp", "").toString();

    m_dstBroadcastAddr = QHostAddress::Broadcast;
    if (defIp.length() < 1){
        return;
    }

    foreach(const QNetworkInterface &i, QNetworkInterface::allInterfaces())
    {
        if (!i.flags().testFlag(QNetworkInterface::IsUp) || i.flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            continue;
        }

        foreach (QNetworkAddressEntry entry, i.addressEntries())
        {
            if (entry.ip().protocol() != QAbstractSocket::IPv4Protocol)
            {
                continue;
            }

            if ( i.hardwareAddress() == "00:00:00:00:00:00" || i.humanReadableName().contains("VM") || i.humanReadableName().contains("Virtual"))
            {
                continue;
            }

            QString curIp = entry.ip().toString();
            if (curIp.length() < 4 || !curIp.contains( defIp))
            {
                continue;
            }

            m_sMacAddr = i.hardwareAddress();
            m_dstBroadcastAddr = entry.broadcast();
            break;
        }
    }
}

int DeviceMonitor::broadcastInfo()
{
    QString localAddr = "PhoneLinServer" + m_sMacAddr;
    QUdpSocket udpSocket;

    QByteArray datagram = localAddr.toUtf8();

    if (datagram.size() != udpSocket.writeDatagram(datagram.data(),datagram.size(), m_dstBroadcastAddr , PL_BROADCAST_PORT ))
    {
        DEBUG_INFO  << "broadcast info failed";
        return PL_RET_FAIL;
    }

    return PL_RET_OK;
}


int DeviceMonitor::initSSLData()
{
    QFile certFile(QStringLiteral(":/SSL/ssl/PlRootCA.cert"));
    QFile keyFile(QStringLiteral(":/SSL/ssl/PlRootCAPrivate.pem"));

    if (!certFile.open(QIODevice::ReadOnly))
    {
        DEBUG_INFO  <<  certFile.errorString();
        return PL_RET_FAIL;
    }

    if (!keyFile.open(QIODevice::ReadOnly))
    {
        DEBUG_INFO  <<  certFile.errorString();
        return PL_RET_FAIL;
    }

    m_CACert = new QSslCertificate(&certFile, QSsl::Pem);
    if (m_CACert->isNull()){
        DEBUG_INFO  <<  "can not open cert file";
        return PL_RET_FAIL;
    }

    QString pwd = "!p@h@o#n$e!";
    m_sslPrivateKey = new QSslKey( &keyFile, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey,pwd.toUtf8() );
    if (m_sslPrivateKey->isNull()){
        DEBUG_INFO  <<  "can not open private key file";
        return PL_RET_FAIL;
    }

    certFile.close();
    keyFile.close();

    return PL_RET_OK;
}


void DeviceMonitor::incomingConnection(qintptr socketDescriptor)
{
    DeviceAgent *deviceSock = new DeviceAgent(this);
    if (!deviceSock->setSocketDescriptor(socketDescriptor)) {
        DEBUG_INFO << "Cannot set socket descriptor";
        delete deviceSock;
        return;
    }

    connect(deviceSock, SIGNAL(busyMessage(QString)), this, SIGNAL(busyMessage(QString)));
    connect(deviceSock, SIGNAL(deviceConnected(DeviceAgent*)), this, SLOT(onDeviceConnected(DeviceAgent*)));
    connect(deviceSock, SIGNAL(deviceDisconnected(DeviceAgent*)), this, SLOT(onDeviceDisconnected(DeviceAgent*)));
    connect(deviceSock, SIGNAL(dataReceived(QString &,int,QByteArray*)), this, SLOT(onDataReceived(QString &,int,QByteArray*))/*,Qt::BlockingQueuedConnection*/);
    /*connect(serverSocket, SIGNAL(modeChanged(QSslSocket::SslMode)), this, SLOT(onModeChanged(QSslSocket::SslMode)));
    connect(serverSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onStateChanged(QAbstractSocket::SocketState)));
    connect(serverSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));*/

    QSslConfiguration sslConfiguration = deviceSock->sslConfiguration();
    sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyNone);
    sslConfiguration.setLocalCertificate(*m_CACert); // set domain cert
    sslConfiguration.setPrivateKey(*m_sslPrivateKey); // set domain key
    sslConfiguration.setProtocol(QSsl::AnyProtocol);

    sslConfiguration.setCaCertificates(QList<QSslCertificate>() << *m_CACert); // add ca cert

    deviceSock->setSslConfiguration(sslConfiguration);

    deviceSock->startServerEncryption();

}

void DeviceMonitor::onDeviceConnected(DeviceAgent *device)
{
    DEBUG_INFO << "Enter";
    m_clients.insert(device->m_devID,device);
    emit deviceConnected(device->m_devID);
    emit getSysInfo(device->m_devID);
}

void DeviceMonitor::onDeleteLater(void *obj)
{
    DEBUG_INFO << "Enter";
    delete obj;
}

void DeviceMonitor::onDeviceDisconnected(DeviceAgent *device)
{
    DEBUG_INFO << "Enter";
    QMap<QString, DeviceAgent*>::iterator it = m_clients.find(device->m_devID);
    if (it != m_clients.end())
    {
        DEBUG_INFO << "Enter";
        DeviceAgent* device = reinterpret_cast<DeviceAgent *>(it.value());
        emit deviceDisconnected(device->m_devID);
        m_clients.remove(it.key());
    }

    emit deleteLater(device);

}



void DeviceMonitor::onAppDataComing(QString &devid, int action,QByteArray *data)
{
    JsonHelper jp;
    if (PL_RET_OK !=  jp.parseObject(data))
    {
        DEBUG_INFO  << "parseArray failed";
        return;
    }

    if (action == PL_REQ_DEL_APP || action == PL_REQ_INSTALL_APP)\
    {
        int retcode = jp.m_jsonObj.value("retCode").toInt();
        if (retcode == PL_RET_OK)
        {
            busyMessage("Please proceed on your device");
        }
        else
        {
            QString info = QString("App action %1 failed with code %2").arg(action).arg(retcode);
            busyMessage(info);
        }

        return;
    }

    if (action != PL_REQ_GET_APP_LIST)
    {
        return;
    }

    //jp.dumpObject(jp.m_jsonObj,"applist");

    QJsonArray appList = jp.m_jsonObj.value("data").toArray();
    emit updateAppList(appList);

    for(auto&& item: appList)
    {
        QJsonObject app;// = item.toObject();
        app = item.toObject();
        emit getAppIcon(devid,app.value("packageName").toString());
    }
}


void DeviceMonitor::onPimDataComing(QString &devid, int action,QByteArray *data)
{
    if (data == NULL)
    {
        DEBUG_INFO << "data is null";
        return;
    }

    JsonHelper jp;
    jp.parseObject(data);
    //jp.dumpObject(jp.m_jsonObj,"action.json");

    QJsonArray dataAry = jp.getValue("data").toArray();
    int pim_action = jp.getValue("pim_action").toDouble();
    if (pim_action == PIM_ACTION_SEND_SMS)
    {
        int code = jp.getValue("retCode").toDouble();
        if (code == 0)
        {
            busyMessage("SMS send Done");
        }
        else
        {
            busyMessage("Failed to send SMS");
        }

        return;
    }

    DbHelper *db = new DbHelper();
    int update_view = SMS_VIEW_IDX;

    for(auto&& item: dataAry)
    {
        QJsonObject jsonObj;// = item.toObject();
        jsonObj = item.toObject();

        switch (pim_action)
        {
        case PIM_ACTION_GET_CALL_LIST:
        {
            db->insertCall(jsonObj);
            update_view = CALL_VIEW_IDX;
            break;
        }
        case PIM_ACTION_GET_CONTACT_LIST:
        {
            db->insertContact(jsonObj);

            int id = jsonObj.value("_id").toString().toInt();
            emit getContactData(devid,id);

            update_view = CONTACT_VIEW_IDX;

            break;
        }
        case PIM_ACTION_GET_SMS_LIST:
        {
            db->insertSms(jsonObj);
            update_view = SMS_VIEW_IDX;
            break;
        }
        case PIM_ACTION_GET_SIM_CONTACT_LIST:
        {
            update_view = SMS_VIEW_IDX;
            break;
        }
        case PIM_ACTION_GET_CONTACT_DATA_LIST:
        {
            if (!jsonObj.contains("mimetype"))
            {
                break;
            }

            db->insertContactData(jsonObj);
            update_view = CONTACT_VIEW_IDX;
            break;
        }

#if 0
        case PIM_ACTION_GET_CONTACT_EMAIL_LIST:
        {
            if (!jsonObj.contains("mimetype"))
            {
                break;
            }


            db.insertContactData(jsonObj);

            break;
        }
        case PIM_ACTION_GET_CONTACT_PHONE_LIST:
        {
            if (!jsonObj.contains("mimetype"))
            {
                break;
            }

            db.insertContactData(jsonObj);
            break;
        }
        case PIM_ACTION_GET_CONTACT_ADDR_LIST:
        {
            if (!jsonObj.contains("mimetype"))
            {
                break;
            }

            db.insertContactData(jsonObj);

            break;
        }
#endif
        }
    }


    emit updateView(update_view);

    return;
}




int DeviceMonitor::getRetCodeFromJson(QJsonDocument parse_doucment)
{
    if(!parse_doucment.isObject())
    {
        DEBUG_INFO << "Parse Json data failed: not object";
        return PL_RET_FAIL;
    }

    QJsonObject ret = parse_doucment.object();
    int retcode = ret.value("retCode").toInt();
    return retcode;
}

void DeviceMonitor::onStorageDataComing(QString &devID,int action, QByteArray *data)
{
    if (data == NULL)
    {
        QString error = QString("File explorer error:%1").arg(action);
        emit errorMessage("Error",error);
        emit refreshFileList();
        return;
    }

    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(*data,&json_error);
    if(json_error.error != QJsonParseError::NoError)
    {
        DEBUG_INFO << "Parse Json data failed:" << json_error.error;
        return;
    }


    switch (action)
    {
    case PL_REQ_DOWNLOAD_FILE:
    case PL_REQ_RENAME_FILE:
    case PL_REQ_DEL_FILE:
    case PL_REQ_NEW_FILE:
    case PL_REQ_UPLOAD_FILE:
    case PL_REQ_COPY_FILE:
    case PL_REQ_CUT_FILE:
    {
        int ret = getRetCodeFromJson(parse_doucment);
        if (ret == 0)
        {
            busyMessage("File operation done");
        }
        else
        {
            QString info = QString("File operation failed with code %1").arg(ret);
            busyMessage(info);
        }

        emit refreshFileList();

        break;
    }
    case PL_REQ_GET_SD_CARD_LIST:
    {
        if(!parse_doucment.isArray())
        {
            DEBUG_INFO << "Parse Json data failed: not object";
            return;
        }

        //m_fileView->onSdCartListComing(parse_doucment.array());
        emit sdCartListComing(parse_doucment.array());
        break;
    }
    case PL_REQ_GET_FILE_LIST:
    {
        if(!parse_doucment.isObject())
        {
            DEBUG_INFO << "Parse Json data failed: not object";
            return;
        }

        emit fileListUpdated(parse_doucment.object());
        break;
    }
    }

}

bool DeviceMonitor::isMediaItemExistsOnDisk(QString path)
{
    QFile mediaFile(path);
    if (mediaFile.exists())
    {
        return true;
    }

    return false;
}


void DeviceMonitor::onMediaDataComing(QString &devID,int action, QByteArray *data)
{
    if (data == NULL)
    {
        DEBUG_INFO << "data is null";
        return;
    }

    JsonHelper jp;
    jp.parseObject(data);

    //PimProxy helper(m_monitor,devid);

    QJsonArray dataAry = jp.getValue("data").toArray();

    int pim_action = jp.getValue("media_action").toInt();
    jp.dumpObject(jp.m_jsonObj,"media");

    QString media_path = "";
    QString file_name = "";
    for(auto&& item: dataAry)
    {
        QJsonObject jsonObj;// = item.toObject();
        jsonObj = item.toObject();

        if (jsonObj.contains("_data"))
        {
            media_path = jsonObj.value("_data").toString();
        }
        if (jsonObj.contains("_display_name"))
        {
            file_name = jsonObj.value("_display_name").toString();
        }

        QString subFolder = "";
        switch (pim_action)
        {
        case MEDIA_ACTION_GET_IMAGE_LIST:
        {
            subFolder = "image";
            break;
        }
        case MEDIA_ACTION_GET_VIDEO_LIST:

        {
            subFolder = "video";
            break;
        }
        case MEDIA_ACTION_GET_AUDIO_LIST:
        {
            subFolder = "audio";
            break;
        }

        }

        if (m_sMediaSyncFolder.isEmpty() || media_path.isEmpty() || file_name.isEmpty() || subFolder.isEmpty())
        {
            continue;
        }

        QString local_file_path = util_make_path(m_sMediaSyncFolder , subFolder);
        if (!QDir(local_file_path).exists())
        {
            QDir().mkdir(local_file_path);
        }

        local_file_path = util_make_path(local_file_path , file_name);
        if (isMediaItemExistsOnDisk(local_file_path))
        {
            continue;
        }

        //download media item
        emit downloadFile(devID,media_path,local_file_path);

    }

    return;
}



void DeviceMonitor::onEventReceived(QString &devID,int action,QByteArray *data)
{
    JsonHelper jp;
    if (PL_RET_OK != jp.parseObject(data))
    {
        return;
    }

    QString evt = jp.getValue("evt").toString();

    if (evt.isEmpty())
    {
        return;
    }

    if (evt.contains("SYS_EVT_SMS_SENT_STATUS:"))
    {
        evt = evt.replace("SYS_EVT_SMS_SENT_STATUS","");
    }
    else if (evt.contains("SYS_EVT_SMS_DELIVER_STATUS:"))
    {
        evt = evt.replace("SYS_EVT_SMS_DELIVER_STATUS","");
    }
    else if (evt.contains("SYS_EVT_BATTERY_LEVEL_CHANGED"))
    {
        emit getSysInfo(devID);
    }
    else if (evt.contains("SYS_EVT_CALL_LOG_CHANGED"))
    {
        emit loadDevicePimData(devID);
    }
    else if (evt.contains("SYS_EVT_SMS_RECEIVED"))
    {
        emit loadDevicePimData(devID);
    }

}

void DeviceMonitor::onDataReceived(QString &devID,int action,QByteArray *data)
{
    busyMessage("Data received");
    if (data == NULL)
    {
        busyMessage("Done");
        //return;
    }

    switch (action)
    {
    case PL_REQ_POST_EVENT:
    {
        emit busyMessage("Event received");
        onEventReceived(devID,action,data);
        break;
    }
    case PL_REQ_GET_SYS_INFO:
    {
        DbHelper::m_curDeviceID = devID;

        JsonHelper jp;
        if (PL_RET_OK != jp.parseObject(data))
        {
            break;
        }

        QString model = jp.getValue("ProductModel").toString();
        DbHelper::insertDevice(model,devID);
        emit showDeviceInfo(jp.m_jsonObj);

        static int iFirst = 1;
        if (iFirst)
        {
            iFirst = 0;
            emit loadDevicePimData(devID);

            if (m_bAutoSyncMedia && !m_sMediaSyncFolder.isEmpty())
            {
                emit loadMediaData(devID);
            }
            else
            {
                DEBUG_INFO << "Will not auto sync media";
            }

            emit loadSDCardList(devID);
        }
        break;
    }
    case PL_REQ_SEND_SMS:
    case PL_REQ_GET_PIM_DATA:
    {
        emit busyMessage("PIM data coming");
        onPimDataComing(devID,action,data);
        break;
    }
    case PL_REQ_GET_PIM_PHOTO:
    {
        char contact_id[255];
        memcpy(contact_id,data->data(),255);

        char *iconData = (char *)data->data() + 255;
#if 0
        QPixmap pixmap;
        bool ret = pixmap.loadFromData((uchar*)iconData,data->length() - 255);
        if (!ret)
        {
            DEBUG_INFO << contact_id << ":invalid icon data";
            break;
        }
#endif
        QString name = util_make_path(QDir::tempPath(),contact_id) + ".png";

        QFile file(name);
        if (file.open(QIODevice::WriteOnly))
        {
            file.write(iconData,data->length() - 255);
            file.close();
            QString contactID = contact_id;
            emit updateContactIcon(contactID,name);
        }

        break;
    }
    case PL_REQ_DEL_APP:
    case PL_REQ_INSTALL_APP:
    case PL_REQ_GET_APP_LIST:
    {
        onAppDataComing(devID,action,data);
        break;
    }
    case PL_REQ_GET_APP_ICON:
    {
        char packageName[255];
        memcpy(packageName,data->data(),255);

        char *iconData = (char *)data->data() + 255;

        QString name = util_make_path(QDir::tempPath(),packageName) + ".png";
        QFile file(name);
        bool bExist = file.exists();
        if (file.open(QIODevice::WriteOnly))
        {
            file.write(iconData,data->length() - 255);
            file.close();

            if (!bExist)
            {
                QString pkg = packageName;
                emit updateAppIcon(pkg,name);
                busyMessage("Update app Icon");
            }
        }

        break;
    }
    case PL_REQ_DOWNLOAD_FILE:
    case PL_REQ_RENAME_FILE:
    case PL_REQ_DEL_FILE:
    case PL_REQ_NEW_FILE:
    case PL_REQ_UPLOAD_FILE:
    case PL_REQ_GET_SD_CARD_LIST:
    case PL_REQ_GET_FILE_LIST:
    case PL_REQ_COPY_FILE:
    case PL_REQ_CUT_FILE:
    {
        onStorageDataComing(devID,action,data);
        break;
    }
    case PL_REQ_GET_MEDIA_LIST:
    {
        busyMessage("Media List coming");
        onMediaDataComing(devID,action,data);
        break;
    }

    }

    return;
}



int DeviceMonitor::sendCmd(QString &devID,uint32_t action,QByteArray *data)
{
    QMap<QString, DeviceAgent*>::iterator it = m_clients.find(devID);
    if (it != m_clients.end())
    {
        DeviceAgent* pDevice = reinterpret_cast<DeviceAgent *>(it.value());
        //QApplication::setOverrideCursor(Qt::WaitCursor);
        emit busyMessage("Chat with device...");
        pDevice->sendData(action, data);
    }

    return PL_RET_OK;
}



int DeviceMonitor::sendFile(QString &devID,uint32_t action,QString &localFileName,QString &dstFileName)
{
    QMap<QString, DeviceAgent*>::iterator it = m_clients.find(devID);
    if (it != m_clients.end())
    {
        DeviceAgent* pDevice = reinterpret_cast<DeviceAgent *>(it.value());
        //QApplication::setOverrideCursor(Qt::WaitCursor);
        emit busyMessage("Chat with device...");
        pDevice->sendFile(action, localFileName,dstFileName);
    }

    return PL_RET_OK;
}



void DeviceMonitor::onLoadSDCardList(QString devid)
{
    StorageProxy proxy(this,devid);
    proxy.getSDCardList();
}



void DeviceMonitor::onLoadDevicePimData(QString devid)
{
    PimProxy helper(this,devid);

    ID_TYPE max_id = 0;
    if (PL_RET_OK == DbHelper::getCallLogMaxId(max_id))
    {
        ++max_id;
        helper.getCallList(max_id);
    }

    max_id = 0;
    if (PL_RET_OK == DbHelper::getSmsMaxId(max_id))
    {
        ++max_id;
        helper.getSmsList(max_id);
    }

    max_id = 0;
    if (PL_RET_OK == DbHelper::getContactMaxId(max_id))
    {
        ++max_id;
        helper.getContactList(max_id);
    }

}



void DeviceMonitor::onInstallApp(QString devID,QString apkPath)
{
    AppProxy appProxy(this,devID);
    appProxy.installApp(apkPath);
}


void DeviceMonitor::onRmApp(QString devID,QString packageName)
{
    AppProxy appProxy(this,devID);
    appProxy.delApp(packageName);

}


void DeviceMonitor::onRefreshAppList(QString devID,int from)
{
    AppProxy appProxy(this,devID);
    appProxy.getAppList(from);
}


void DeviceMonitor::onLoadMediaData(QString devID)
{
    QSettings settings("PhoneLinSettings.ini", QSettings::IniFormat);
    m_sMediaSyncFolder = settings.value("mediaSyncFolder", "").toString();
    if (m_sMediaSyncFolder.isEmpty())
    {
        DEBUG_INFO << "Can not sync Media data: please specify the media folder first";
        busyMessage("Can not sync Media data: please specify the media folder first");
        return;
    }

    busyMessage("Sync Media data now");
    StorageProxy proxy(this,devID);
    proxy.getAudioList(0);
    proxy.getVideoList(0);
    proxy.getImageList(0);
}



void DeviceMonitor::onGetFileList(QString devID,QString path,int from)
{
    StorageProxy proxy(this,devID);
    proxy.getFileList(path,from);
}



void DeviceMonitor::onNewFile(QString devID,QString path,bool isDir)
{
    StorageProxy proxy(this,devID);
    proxy.newFile(path,isDir);
}


void DeviceMonitor::onRenameFile(QString devID,QString src_path,QString dst_path)
{
    StorageProxy proxy(this,devID);
    proxy.renameFile(src_path,dst_path);


}

void DeviceMonitor::onDownloadFile(QString devID,QString src_path,QString local_dst_path)
{
    StorageProxy proxy(this,devID);
    proxy.downloadFile(src_path,local_dst_path);
}

void DeviceMonitor::onUploadFile(QString devID,QString src_path,QString dst_path)
{
    StorageProxy proxy(this,devID);
    proxy.uploadFile(src_path,dst_path);

}

void DeviceMonitor::onRemoveFile(QString devID,QString path)
{
    StorageProxy proxy(this,devID);
    proxy.delFile(path);

}

void DeviceMonitor::onSendSMS(QString devID,QString recipients,QString content)
{
    PimProxy proxy(this,devID);
    proxy.sendSms(recipients,content);
}




void DeviceMonitor::onCopyFile(QString devID,int op, QString src_path,QString dst_path)
{
    StorageProxy proxy(this,devID);

    if (op == FILE_OP_CUT)
    {
        proxy.cutFile(src_path,dst_path);

    }
    else if (op == FILE_OP_COPY)
    {

        proxy.copyFile(src_path,dst_path);
    }

}


void DeviceMonitor::onGetSysInfo(QString devID)
{
    sendCmd(devID,PL_REQ_GET_SYS_INFO,NULL);
}

void DeviceMonitor::onGetAppIcon(QString devID,QString pkgName)
{
    AppProxy appProxy(this,devID);
    appProxy.getAppIcon(pkgName);
}

void DeviceMonitor::onGetContactData(QString devID, int contact_id)
{
    PimProxy helper(this,devID);

    helper.getContactPhoto(contact_id);

    helper.getContactDataList(contact_id);
#if 0
    helper.getContactAddress(id);
    helper.getContactEmailList(id);
    helper.getContactPhoneList(id);
#endif
}



