
#ifndef __TEST_MONITOR_HEADER__
#define __TEST_MONITOR_HEADER__

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QtNetwork/QSslError>
#include <QtNetwork/QTcpServer>
#include <QString>
#include <QtNetwork/QSsl>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>

#include "phonelin.h"
#include "controler/netPackage.h"
#include "controler/deviceAgent.h"

#include "controler/deviceMonitor.h"


class TestMonitor:public DeviceMonitor
{
   Q_OBJECT
public:
   explicit TestMonitor(QObject *parent);

    int sendCmd(QString &devID,uint32_t action,QByteArray *data);

Q_SIGNALS:
    void dataReceived(QString &devID,int action,QByteArray *data);
    void deviceDisconnected(DeviceAgent *dev);
    void deviceConnected(DeviceAgent *dev);


protected slots:
    int onStartTest();

private:
    int loadJson(QString &fileName);
    int isJsonObject(QByteArray data);
    int isJsonArray(QByteArray data);
	
    int loadJsonObject(QString &fileName,QJsonObject &jsonObj );
    int loadJsonArray(QString &fileName,QJsonArray &jsonAry );

    int pimTest(QString &devID,uint32_t action,QByteArray *data);
	int generalTest(QString &devID,uint32_t action,QByteArray *data);

    pthread_mutex_t conn_mutex = PTHREAD_MUTEX_INITIALIZER;

};


#endif



