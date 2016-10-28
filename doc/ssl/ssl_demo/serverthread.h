#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H
#include <QThread>
#include <QSslSocket>
#include <QSslCipher>
#include <QTimer>
class ServerThread : public QThread
{
    Q_OBJECT
public:
    ServerThread(int socketDescriptor, QObject *parent = 0);
    ~ServerThread();
    int getSocketDescriptor() const;
    int getInitSocketDescriptor() const;
protected:
    void run();

private slots:
    void _run();
    void datareceive();
    void handleSocketEncrypted();
    void handleSslModeChanged(QSslSocket::SslMode mode);
    void handleSocketConnected();
    void handleSocketDisconnected();
    void handleSockStateChange(const QAbstractSocket::SocketState &socketState);

    void handleSocketError(const QAbstractSocket::SocketError &socketError);
    void handleSslErrorList(const QList<QSslError> &errorList);
    void handlePeerVerifyError(const QSslError &error);
    void handleSslError(const QSslError &error);

private:
    void requestDestroyed();
signals:
    void socketDisconnected(int initDescriptor);
private:
    int socketDescriptor;
    QSslSocket *socket;
    QTimer *runTimer;
};

#endif // SERVERTHREAD_H

