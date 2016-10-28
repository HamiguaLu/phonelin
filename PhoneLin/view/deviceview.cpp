#include "deviceview.h"
#include "ui_device.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QMovie>
#include <QDir>
//#include <QHostAddress>
#include <QNetworkInterface>
#include <QSettings>

DeviceView::DeviceView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceView)
{
    ui->setupUi(this);

    ui->m_batteryLevel->setRange(0,100);
    ui->m_batteryLevel->setTextVisible(true);

    ui->m_ramLevel->setRange(0,100);
    ui->m_ramLevel->setTextVisible(true);

    m_movie = new QMovie(":/icon/icon/loading.gif");
    m_onlineIcon = new QPixmap(":/icon/icon/android_online.png");
    m_offlineIcon = new QPixmap(":/icon/icon/android_offline.png");

    m_NetworkManager = new QNetworkAccessManager(this);
    QObject::connect(m_NetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onNetworkReply(QNetworkReply*)));


    onDeviceDisconnected();

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

            if ( i.hardwareAddress() != "00:00:00:00:00:00" && !i.humanReadableName().contains("VM") && !i.humanReadableName().contains("Virtual"))
            {
                QString addr = i.humanReadableName() + " " + entry.ip().toString();
                ui->m_comboBoxIpList->addItem(addr);
            }
        }

    }

    QSettings settings("PhoneLinSettings.ini", QSettings::IniFormat);
    QString defIp = settings.value("defIp", "").toString();
    if (defIp.length() < 1)
    {
        ui->m_comboBoxIpList->setCurrentIndex(0);
        defIp = ui->m_comboBoxIpList->itemData(0).toString();
        settings.setValue("defIp",defIp);
    }
    else
    {
        for (int i = 0; i < ui->m_comboBoxIpList->count(); ++i)
        {
            QString item = ui->m_comboBoxIpList->itemData(i).toString();
            if (item.contains(defIp))
            {
                ui->m_comboBoxIpList->setCurrentIndex(i);
                break;
            }
        }
    }

    if (defIp.length() > 4)
    {
        genQRCodeOnline(defIp);
    }

    QObject::connect( ui->m_comboBoxIpList, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(onCurrentIpChanged(const QString&)));

}

DeviceView::~DeviceView()
{
    delete ui;
}


void DeviceView::onShowDeviceInfo(QJsonObject sysInfo)
{
    int battery_level = sysInfo.value("BatteryLevel").toString().toInt();
    ui->m_batteryLevel->setValue(battery_level);

    int availRAM = sysInfo.value("AvailRAM").toString().toInt();
    int totalRAM = sysInfo.value("TotalRAM").toString().toInt();

    if (totalRAM == 0)
    {
        ui->m_ramLevel->setValue(0);
    }
    else
    {
        ui->m_ramLevel->setValue(availRAM * 100/totalRAM);
    }

    QString model = sysInfo.value("ProductModel").toString();
    ui->m_PhoneModel->setText(model);

    ui->m_conn_Indicator->setPixmap(*m_onlineIcon);

    m_movie->stop();
    ui->m_loadingProcess->hide();
    ui->m_loadingLabel->hide();

    repaint();
}


void DeviceView::onDeviceDisconnected()
{
    ui->m_batteryLevel->setValue(0);
    ui->m_ramLevel->setValue(0);

    ui->m_PhoneModel->setText("");

    ui->m_conn_Indicator->setPixmap(*m_offlineIcon);

    ui->m_loadingProcess->show();
    ui->m_loadingLabel->show();
    ui->m_loadingProcess->setMovie(m_movie);
    m_movie->start();

    repaint();

}


void DeviceView::genQRCodeOnline(QString ip)
{
    QUrl url("http://qrickit.com/api/qr?d="+ip);
    m_NetworkManager->get(QNetworkRequest(url));
}



void DeviceView::onNetworkReply(QNetworkReply* reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        int httpstatuscode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toUInt();
        switch(httpstatuscode)
        {
        case 200:
            if (reply->isReadable())
            {
                QString name = util_make_path(QDir::tempPath(),"PL_HOST_CONN_QRCode") + ".png";
                QPixmap *qr = new QPixmap(name);
                ui->m_labelQrCode->setPixmap(*qr);
                QFile file(name);
                if (file.open(QIODevice::WriteOnly))
                {
                    QByteArray qrCodeData = reply->readAll();
                    if (qrCodeData.length() > 0)
                    {
                        file.write(qrCodeData.data(),qrCodeData.length());
                        file.close();
                        QPixmap *qr = new QPixmap();
                        qr->load(name);
                        ui->m_labelQrCode->setPixmap(*qr);
                    }

                }
            }
            break;
        case 404:
            //case RESPONSE_BAD_REQUEST:
        default:
            break;
        }
    }

    reply->deleteLater();
}




void DeviceView::onCurrentIpChanged(const QString &arg1)
{
    QString selIp = arg1;

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

            QString defIp = entry.ip().toString();
            if (defIp.length() < 4 || !selIp.contains( defIp))
            {
                continue;
            }

            QSettings settings("PhoneLinSettings.ini", QSettings::IniFormat);
            settings.setValue("defIp",defIp);
            emit netInterfaceChanged();
            break;
        }
    }

}



