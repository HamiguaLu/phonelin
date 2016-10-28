#include "serverthread.h"
#include "include/debug/debug.h"
#include "getsslrelateinfo.h"
#include "crccheck.h"
#include "datatype.h"
#include "datatransfer.h"
ServerThread::ServerThread(int socketDescriptor, QObject *parent) : QThread(parent), socketDescriptor(socketDescriptor)
{
    socket = new QSslSocket(this);
    runTimer = new QTimer(this);
    moveToThread(this);
}

ServerThread::~ServerThread()
{
}

int ServerThread::getSocketDescriptor() const
{
    if(socket)
        return socket->socketDescriptor();
    return -1;
}

int ServerThread::getInitSocketDescriptor() const
{
    return socketDescriptor;
}

void ServerThread::run()
{
    if (!socket->setSocketDescriptor(socketDescriptor)) {
        PrintMsg("socket->setSocketDescriptor %s",socket->errorString().toAscii().data());
        handleSocketDisconnected();
        return;
    } 

#if 1 /*如果不设定证书和私钥,连接会失败，并不像官方所说那样，可以按不加密方式使用*/
    socket->setPrivateKey("../certificates/server.pem");
    socket->setLocalCertificate("../certificates/server.crt");
#else //for test, self signed certificates
    socket->setPrivateKey("test_certificates/server.key");
    socket->setLocalCertificate("test_certificates/server.csr");

    //socket->setPrivateKey("test_certificates/serverlo.key");
    //socket->setLocalCertificate("test_certificates/serverlo.csr");
#endif

    socket->setPeerVerifyMode(QSslSocket::VerifyPeer);

    connect(socket,SIGNAL(readyRead()),this,SLOT(datareceive()));
    connect(socket,SIGNAL(encrypted()),this,SLOT(handleSocketEncrypted()));
    connect(socket,SIGNAL(modeChanged(QSslSocket::SslMode)),this,SLOT(handleSslModeChanged(QSslSocket::SslMode)));
    connect(socket,SIGNAL(connected()),this,SLOT(handleSocketConnected()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(handleSocketDisconnected()));
    connect(socket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(handleSockStateChange(QAbstractSocket::SocketState)));
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(handleSocketError(QAbstractSocket::SocketError)));
    connect(socket,SIGNAL(sslErrors(QList<QSslError>)),this,SLOT(handleSslErrorList(QList<QSslError>)),Qt::DirectConnection);
    connect(socket,SIGNAL(peerVerifyError(QSslError)),this,SLOT(handlePeerVerifyError(QSslError)));
    socket->startServerEncryption();
    runTimer->setInterval(1000);
    runTimer->setSingleShot(true);
    connect(runTimer,SIGNAL(timeout()),this,SLOT(_run()));
    runTimer->start();
    PrintMsg("socket Mode: %d",socket->mode());
    exec();
}

void ServerThread::_run()
{

    if(socket->isEncrypted())
    {
        QString str = QString("hello I am Server threadId: %1").arg(QThread::currentThreadId());
        QByteArray block;
        block.resize(str.length() + 1);
        memcpy(block.data(),str.toLocal8Bit().data(),block.size());
        socket->write(block);
    }
    runTimer->start();
}

void ServerThread::datareceive()
{
    while(socket->bytesAvailable() > 0 )
    {
        QByteArray datagram;
        datagram.resize(socket->bytesAvailable());
        socket->read(datagram.data(),datagram.size());
        qDebug() << "thread:" << QThread::currentThreadId() << datagram;
    }
}

void ServerThread::handleSocketEncrypted()
{
    PrintMsg("handleSocketEncrypted.");
}

void ServerThread::handleSslModeChanged(QSslSocket::SslMode mode)
{
    PrintMsg("socket Mode Change: %d",mode);
}

void ServerThread::handleSocketConnected()
{
    PrintMsg("Socket Connected");
}

void ServerThread::handleSocketDisconnected()
{
    PrintMsg("Socket Disconnected");
    runTimer->stop();
    requestDestroyed();
}

void ServerThread::requestDestroyed()
{
    PrintMsg("requestDestroyed.bytesToWrite %lld",socket->bytesToWrite());
    if(socket->isOpen())
        socket->close();
    quit();
    emit socketDisconnected(socketDescriptor);
}



void ServerThread::handleSocketError(const QAbstractSocket::SocketError &socketError)
{
    PrintMsg("================socket error=================");
    switch(socketError)
    {
    case QAbstractSocket::ConnectionRefusedError:
        PrintMsg("The connection was refused by the peer (or timed out).");
        break;
    case QAbstractSocket::RemoteHostClosedError:
        PrintMsg("The remote host closed the connection. Note that the client socket (i.e., this socket) will be closed after the remote close notification has been sent.");
        break;
    case QAbstractSocket::HostNotFoundError:
        PrintMsg("The host address was not found.");
        break;
    case QAbstractSocket::SocketAccessError:
        PrintMsg("The socket operation failed because the application lacked the required privileges.");
        break;
    case QAbstractSocket::SocketResourceError:
        PrintMsg("The local system ran out of resources (e.g., too many sockets).");
        break;
    case QAbstractSocket::SocketTimeoutError:
        PrintMsg("The socket operation timed out.");
        break;
    case QAbstractSocket::DatagramTooLargeError:
        PrintMsg("The datagram was larger than the operating system's limit (which can be as low as 8192 bytes).");
        break;
    case QAbstractSocket::NetworkError:
        PrintMsg("An error occurred with the network (e.g., the network cable was accidentally plugged out).");
        break;
    case QAbstractSocket::AddressInUseError:
        PrintMsg("The address specified to bind() is already in use and was set to be exclusive.");
        break;
    case QAbstractSocket::SocketAddressNotAvailableError:
        PrintMsg("The address specified to bind() does not belong to the host.");
        break;
    case QAbstractSocket::UnsupportedSocketOperationError:
        PrintMsg("The requested socket operation is not supported by the local operating system (e.g., lack of IPv6 support).");
        break;
    case QAbstractSocket::ProxyAuthenticationRequiredError:
        PrintMsg("The socket is using a proxy, and the proxy requires authentication.");
        break;
    case QAbstractSocket::SslHandshakeFailedError:
        PrintMsg("The SSL/TLS handshake failed, so the connection was closed (only used in SslSocket)");
        break;
    case QAbstractSocket::UnfinishedSocketOperationError:
        PrintMsg("Used by AbstractSocketEngine only, The last operation attempted has not finished yet (still in progress in the background).");
        break;
    case QAbstractSocket::ProxyConnectionRefusedError:
        PrintMsg("Could not contact the proxy server because the connection to that server was denied");
        break;
    case QAbstractSocket::ProxyConnectionClosedError:
        PrintMsg("The connection to the proxy server was closed unexpectedly (before the connection to the final peer was established)");
        break;
    case QAbstractSocket::ProxyConnectionTimeoutError:
        PrintMsg("The connection to the proxy server timed out or the proxy server stopped responding in the authentication phase.");
        break;
    case QAbstractSocket::ProxyNotFoundError:
        PrintMsg("The proxy address set with setProxy() (or the application proxy) was not found.");
        break;
    case QAbstractSocket::ProxyProtocolError:
        PrintMsg("The connection negotiation with the proxy server because the response from the proxy server could not be understood.");
        break;
    case QAbstractSocket::UnknownSocketError:
        PrintMsg("An unidentified error occurred.");
        break;
    default:
        PrintMsg("Not match any errors.");
        break;
    }
}

void ServerThread::handleSslErrorList(const QList<QSslError> &errorList)
{
    PrintMsg("====================Ssl Error List==================");
    for(int i = 0 ;i < errorList.count(); i++)
    {
        QSslError SslError = errorList.at(i);
        handleSslError(SslError);
    }
}

void ServerThread::handlePeerVerifyError(const QSslError &error)
{
    handleSslError(error);
}

void ServerThread::handleSslError(const QSslError & error)
{
    PrintMsg("====================Ssl Error==================");
    PrintMsg("%s",error.errorString().toAscii().data());
    switch(error.error())
    {
    case QSslError::NoError:
        break;
    case QSslError::UnableToGetIssuerCertificate:
        break;
    case QSslError::UnableToDecryptCertificateSignature:
        break;
    case QSslError::UnableToDecodeIssuerPublicKey:
        break;
    case QSslError::CertificateSignatureFailed:
        break;
    case QSslError::CertificateNotYetValid:
        break;
    case QSslError::CertificateExpired:
        break;
    case QSslError::InvalidNotBeforeField:
        break;
    case QSslError::InvalidNotAfterField:
        break;
    case QSslError::SelfSignedCertificate:
        break;
    case QSslError::SelfSignedCertificateInChain:
        break;
    case QSslError::UnableToGetLocalIssuerCertificate:
        break;
    case QSslError::UnableToVerifyFirstCertificate:
        break;
    case QSslError::CertificateRevoked:
        break;
    case QSslError::InvalidCaCertificate:
        break;
    case QSslError::PathLengthExceeded:
        break;
    case QSslError::InvalidPurpose:
        break;
    case QSslError::CertificateUntrusted:
        break;
    case QSslError::CertificateRejected:
        break;
    case QSslError::SubjectIssuerMismatch:
        break;
    case QSslError::AuthorityIssuerSerialNumberMismatch:
        break;
    case QSslError::NoPeerCertificate:
        break;
    case QSslError::HostNameMismatch:
        break;
    case QSslError::UnspecifiedError:
        break;
    case QSslError::NoSslSupport:
        break;
    default:
        PrintMsg("no match any ssl error.");
        break;
    }
}

void ServerThread::handleSockStateChange(const QAbstractSocket::SocketState &socketState)
{
    PrintMsg("=============Socket State==============");
    switch(socketState)
    {
    case QAbstractSocket::UnconnectedState:
        PrintMsg("The socket is not connected");
        break;
    case QAbstractSocket::HostLookupState:
        PrintMsg("The socket is performing a host name lookup");
        break;
    case QAbstractSocket::ConnectingState:
        PrintMsg("The socket has started establishing a connection");
        break;
    case QAbstractSocket::BoundState:
        PrintMsg("The socket is bound to an address and port (for servers).");
        break;
    case QAbstractSocket::ClosingState:
        PrintMsg("The socket is about to close (data may still be waiting to be written).");
        break;
    case QAbstractSocket::ListeningState:
        PrintMsg("ListeningState .For internal use only.");
        break;
    default:
        PrintMsg("Not match any states.");
        break;
    }
}

