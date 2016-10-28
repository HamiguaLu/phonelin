
#ifndef __DEVICE_MONITOR_HEADER__
#define __DEVICE_MONITOR_HEADER__

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QtNetwork/QSslError>
#include <QtNetwork/QTcpServer>
#include <QString>
#include <QtNetwork/QSsl>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>
#include <QTimer>

#include "phonelin.h"
#include "netPackage.h"
#include "deviceAgent.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonArray>


class DeviceMonitor: public QTcpServer
{
    Q_OBJECT
public:
    explicit DeviceMonitor(QObject *parent);
    virtual ~DeviceMonitor();

    void start();

    virtual int sendCmd(QString &devID,uint32_t action,QByteArray *data);
    virtual int sendFile(QString &devID,uint32_t action,QString &localFileName,QString &dstFileName);

protected:
    int startServer();

    int m_iPort = PL_SERVER_PORT;
    QSslCertificate *m_CACert = NULL;
    QSslKey *m_sslPrivateKey = NULL;
    QHostAddress m_dstBroadcastAddr;
    QString m_sMacAddr;

    int initSSLData();
private slots:
    void onGetSysInfo(QString devid);

    void onGetAppIcon(QString devID,QString pkgName);
    void onInstallApp(QString devid,QString apkPath);
    void onRmApp(QString devid,QString packageName);
    void onRefreshAppList(QString devid,int from);

    void onLoadMediaData(QString devID);

    void onLoadSDCardList(QString devid);

    void onLoadDevicePimData(QString devid);
    void onGetContactData(QString devID, int contact_id);

    void onGetFileList(QString devID,QString path,int from);
    void onNewFile(QString devID,QString path,bool isDir);
    void onRenameFile(QString devID,QString src_path,QString dst_path);
    void onDownloadFile(QString devID,QString src_path,QString local_dst_path);
    void onUploadFile(QString devID,QString src_path,QString dst_path);
    void onRemoveFile(QString devID,QString path);
    void onCopyFile(QString devID,int op, QString src_path,QString dst_path);
    void onSendSMS(QString devID,QString recipients,QString content);

Q_SIGNALS:
    void getSysInfo(QString devid);

    void getAppIcon(QString devID,QString pkgName);
    void installApp(QString devid,QString apkPath);
    void rmApp(QString devid,QString packageName);
    void refreshAppList(QString devid,int from);

    void loadMediaData(QString devID);

    void loadSDCardList(QString devid);
    void loadDevicePimData(QString devid);
    void getContactData(QString devID, int contact_id);

    void getFileList(QString devID,QString path,int from);
    void newFile(QString devID,QString path,bool isDir);
    void renameFile(QString devID,QString src_path,QString dst_path);
    void downloadFile(QString devID,QString src_path,QString local_dst_path);
    void uploadFile(QString devID,QString src_path,QString dst_path);
    void removeFile(QString devID,QString path);
    void copyFile(QString devID,int op, QString src_path,QString dst_path);
    void sendSMS(QString devID,QString recipients,QString content);
private:

    bool isMediaItemExistsOnDisk(QString path);
    int getRetCodeFromJson(QJsonDocument parse_doucment);
    bool m_bAutoSyncMedia = false;
    QString m_sMediaSyncFolder = "";

    QTimer *m_timer;

public slots:
    void onDeviceConnected(DeviceAgent *device);
    void onDeviceDisconnected(DeviceAgent *device);
    void onDeleteLater(void *obj);
    int broadcastInfo();
    void initBroadcastAddr();


protected:
    QMap<QString, DeviceAgent*> m_clients;
protected:
    void incomingConnection(qintptr socketDescriptor);

private slots:

    void onDataReceived(QString &devID,int action, QByteArray *data);

    void onEventReceived(QString &devID, int action,QByteArray *data);
    void onPimDataComing(QString &devID, int action,QByteArray *data);
    void onAppDataComing(QString &devid, int action,QByteArray *data);
    void onStorageDataComing(QString &devID, int action,QByteArray *data);
    void onMediaDataComing(QString &devID,int action, QByteArray *data);



Q_SIGNALS:
    void deviceDisconnected(QString devid);
    void deviceConnected(QString devid);
    void deleteLater(void*);

    void busyMessage(QString info);
    void errorMessage(QString title,QString error);


    void updateAppList(QJsonArray appList);
    void updateAppIcon(QString packageName, QString iconPath);


    void updateView(int idx);


    void refreshFileList();
    void sdCartListComing(QJsonArray l);
    void fileListUpdated(QJsonObject fileData);

    void showDeviceInfo(QJsonObject devInfo);


    void updateContactIcon(QString contactID, QString iconPath);


};









#endif



