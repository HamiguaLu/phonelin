#include "server.h"
#include "serverthread.h"
#include <stdlib.h>
#include "include/debug/debug.h"
Server::Server(QObject *parent)
    : QTcpServer(parent)
{
    /*设置socket的默认ca数据库*/
    QList<QSslCertificate> caCertList = QSslCertificate::fromPath("../certificates/cacert.pem");
    QSslSocket::setDefaultCaCertificates(caCertList);
}

void Server::startService()
{
    listen(QHostAddress::Any,TCP_PORT);
    PrintMsg("listening...");
}

void Server::stopService()
{
    close();
}

void Server::incomingConnection(int socketDescriptor)
{
    ServerThread *thread = new ServerThread(socketDescriptor);
    connect(thread, SIGNAL(socketDisconnected(int)), this, SLOT(serverThreadDisconnected(int)),Qt::QueuedConnection);
    connect(thread,SIGNAL(finished()),this,SLOT(debugMsgThreadFinished()));
    connect(thread,SIGNAL(destroyed()),this,SLOT(debugMsgThreadDestory()));


    threadListLock.lock();
    serverThreadList.append(thread);
    threadListLock.unlock();
    thread->start();
}

void Server::serverThreadDisconnected(int initSocketDescriptor)
{
    QMutexLocker lock(&threadListLock);
    Q_UNUSED(lock)
    for(int i = 0; i < serverThreadList.count(); i++)
    {  
        if(serverThreadList.at(i)->getInitSocketDescriptor() == initSocketDescriptor)
        {
            PrintMsg("serverThreadList.at(%d)->getSocketDescriptor() = %d",i,serverThreadList.at(i)->getSocketDescriptor());
            serverThreadList.at(i)->wait();
            delete serverThreadList.at(i);
            serverThreadList.removeAt(i);
            PrintMsg("serverThreadList.removeAt(%d) initSocketDescriptor = %d list count = %d",i,initSocketDescriptor,serverThreadList.count());
            return;
        }
    }
    return;
}

void Server::debugMsgThreadDestory()
{
    //PrintMsg("thread destory.");
}

void Server::debugMsgThreadFinished()
{
    //PrintMsg("thread finished.");
}

