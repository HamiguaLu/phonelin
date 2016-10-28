#ifndef DEVICEView_H
#define DEVICEView_H

#include <QWidget>
#include "phonelin.h"
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include "controler/jsonHelper.h"
#include <QPixmap>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>


namespace Ui {
class DeviceView;
}

class DeviceView : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceView(QWidget *parent = 0);
    ~DeviceView();

   

private:
    Ui::DeviceView *ui;
    QMovie *m_movie;
    QPixmap *m_onlineIcon;
    QPixmap *m_offlineIcon;

    QNetworkAccessManager* m_NetworkManager;

    void genQRCodeOnline(QString ip);

public slots:
    void onNetworkReply(QNetworkReply* reply);
    void onCurrentIpChanged(const QString &arg1);
	 void onShowDeviceInfo(QJsonObject sysInfo);
	void onDeviceDisconnected();

Q_SIGNALS:
    void netInterfaceChanged();
};

#endif // DEVICEView_H
