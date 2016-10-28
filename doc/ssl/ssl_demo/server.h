#ifndef SERVER_H
#define SERVER_H
#include "include/common/net_heads.h"
#include <QTcpServer>
#include <QSslSocket>
#include <QSslCipher>
#include <QMutex>
#include <QMutexLocker>

class ServerThread;
class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server(QObject *parent = 0);
public slots:
    void startService();
    void stopService();
signals:

private slots:
    void debugMsgThreadDestory();
    void debugMsgThreadFinished();
    void serverThreadDisconnected(int initSocketDescriptor);
protected:
    void incomingConnection(int socketDescriptor);

private:
    QList<ServerThread *> serverThreadList;
    QMutex threadListLock;
};
#endif // SERVER_H

