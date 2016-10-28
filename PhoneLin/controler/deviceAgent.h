
#ifndef __DEVICE_AGENT_HEADER__
#define __DEVICE_AGENT_HEADER__

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QtNetwork/QSslError>
#include <QtNetwork/QTcpSocket>
#include <QString>
#include <QtNetwork/QSsl>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>
#include <QSslSocket>
#include <QFile>

#include "phonelin.h"
#include "netPackage.h"



class DeviceAgent: public QSslSocket
{
    Q_OBJECT
public:
    explicit DeviceAgent(QObject *parent = Q_NULLPTR);

    ~DeviceAgent();

    int sendFile(uint32_t action,QString &localFileName,QString &dstFileName);
    int sendData(uint32_t action,QByteArray *data);
    QString m_devID;

private:
    pthread_mutex_t conn_mutex = PTHREAD_MUTEX_INITIALIZER;

    NetPackageHdr m_dataHdr;
    qint64 m_dataPos = 0;
    int m_readState = READ_STATE_WAIT_PACKAGE_HEADER;
    qint64 m_datalen = 0;

    QByteArray *m_data = NULL;

    QString m_srcFileName = "";
    QString m_dstFileName = "";
    QFile m_downloadfile;
    int m_fileTransferRetCode = 0;

    int m_iLoopCounter = 0;
Q_SIGNALS:
    void dataReceived(QString &devID,int action,QByteArray *data);
    void deviceDisconnected(DeviceAgent *dev);
    void deviceConnected(DeviceAgent *dev);

    void busyMessage(QString info);


private Q_SLOTS:
    void onDataComing();
    void onDeviceDisconnected();
    void onEncrypted();
    void onSslErrors(const QList<QSslError> &errors);

    void onPackageDataError();
};






#endif


