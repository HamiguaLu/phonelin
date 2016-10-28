#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QToolBar>
#include <QToolButton>
#include <QJsonArray>

#include <QMenu>
#include <QAction>


#include "deviceview.h"
#include "controler/deviceMonitor.h"
#include "controler/monitorThread.h"
#include "db/dbHelper.h"
#include "smsview.h"
#include "calllogview.h"
#include "contactview.h"
#include "appview.h"
#include "fileview.h"
#include "phonelin.h"



namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onShowHome();
	void onShowSMS();
	void onShowCall();
	void onShowContact();
	void onShowApp();
	void onShowFile();

	void onAbout();
    void onSetting();
    void onSyncMedia();

    void onBusyMessage(QString msg);
	
private:
    Ui::MainWindow *ui;

    QVBoxLayout *m_rootLayout;
	QMenu *m_context_menu;
    QAction *m_about_act;
    QAction *m_setting_act;
    QAction *m_sync_media_act;

	QToolButton  *m_btnSetting;

	
    void initToolBar();
    void initSubView();
	void initStatusBar();
	void createActions();

    void setIcon4Btn(QToolButton *btn, QString txt,QString iconPath);

    DeviceView *m_homeView;
    SmsView *m_SmsView;
	ContactView *m_contactView;
	AppView *m_appView;
	CalllogView *m_callView;
	FileView *m_fileView;

    QToolButton *m_btn_home = NULL;
    QToolButton *m_btn_sms = NULL;
    QToolButton *m_btn_call = NULL;
    QToolButton *m_btn_contact = NULL;
    QToolButton *m_btn_app = NULL;
    QToolButton *m_btn_file = NULL;

	void changeSubView(int viewIdx);


	
public slots:
    void onShowErrorInfo(QString title,QString error);
	void onUpdateView(int idx);
    void onDeviceDisconnected(QString devid);
    void onDeviceConnected(QString devid);
    void onMonitorCreated(DeviceMonitor* m);
    
public:
    MonitorThread *m_monitor_thread = NULL;
    DeviceMonitor *m_monitor = NULL;
    QString m_curDevID = "";
};




#endif // MAINWINDOW_H


