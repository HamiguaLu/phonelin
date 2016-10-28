
#include <QApplication>
#include <QFont>
#include <QDesktopWidget>

#include "view/mainwindow.h"

#include "db/dbHelper.h"
#include "view/mimeTypes.h"
#include <QNetworkAddressEntry>
#include <QNetworkInterface>
#include <QSettings>
#include <QFontDatabase>

#ifndef Q_OS_WIN32
#include <QtPlugin>
Q_IMPORT_PLUGIN(QSQLiteDriverPlugin)
Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
#endif

void setIpInterface();

void myMessageHandler(QtMsgType type, const QMessageLogContext &, const QString & msg)
{
    QString txt;
    QDateTime dt = QDateTime::currentDateTime();
    switch (type) {
    case QtDebugMsg:
    case QtWarningMsg:
    case QtCriticalMsg:
    case QtFatalMsg:
        txt = QString("%1: %2").arg(dt.toString()).arg(msg);
    break;
    }
    QFile outFile("PhoneLin.log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}

int main(int argc, char *argv[])
{
    DbHelper::init();
    MimeTypes::init();

    QApplication a(argc, argv);
    qInstallMessageHandler(myMessageHandler);

    QString qss = QString(".QLabel[page=\"true\"] { padding: 1px; }")
            + QString(".QLabel[currentPage=\"true\"] { color: white; border-radius: 4px; background-color:green }")
            + QString(".QLabel[page=\"true\"]:hover { color: white; border-radius: 4px; background-color: qlineargradient(spread:reflect, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(53, 121, 238, 255), stop:1 rgba(0, 202, 237, 255));}");
    //qss += QString(".QWidget{background-color:green}");

    a.setStyleSheet(qss);

#ifdef Q_OS_WIN32
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD( 1, 1 );
    err = WSAStartup( wVersionRequested, &wsaData );
    if ( err != 0 )
    {
        DEBUG_INFO  << "Error init winsock" + err;
    }
#endif


    int fontID = -1;

    QFile res(":/icon/dejavu/DejaVuSerif.ttf");
    if (res.open(QIODevice::ReadOnly) == false)
    {
        qWarning() << "Failed to load font res";
    }
    else
    {
        fontID = QFontDatabase::addApplicationFontFromData(res.readAll());
        if (fontID == -1 )
        {
            qWarning() << "Failed to load font res";
        }
        else
        {
            QApplication::setFont(QFont("DejaVu Serif",10));
        }
    }


    setIpInterface();

    QDesktopWidget dw;
    MainWindow w;

    int x = dw.width()*0.8;
    int y = dw.height()*0.8;
    w.setFixedSize(x,y);

    w.show();

    int ret = a.exec();

    DbHelper::deinit();

    return ret;
}


QString util_make_path(QString folder,QString file)
{
    QString localPath = folder + FILE_PATH_SEPERATOR + file;

    return localPath.replace("//","/").replace("//","/");
}




void setIpInterface()
{
    QSettings settings("PhoneLinSettings.ini", QSettings::IniFormat);
    QString defIp = settings.value("defIp", "").toString();
    int iFound = 0;
findAgain:
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
            if (curIp.length() < 4 )
            {
                continue;
            }

            if (defIp.length() < 1)
            {
                settings.setValue("defIp",curIp);
                iFound = 1;
                break;
            }

            if (curIp.contains(defIp))
            {
                iFound = 1;
                break;
            }
        }
    }

    if (iFound == 0)
    {
        settings.setValue("defIp","");
        defIp = "";
        iFound = 2;
        goto findAgain;
    }

    if (iFound == 2)
    {
        DEBUG_INFO << "Can not find a valid netwrok interface";
    }

}

