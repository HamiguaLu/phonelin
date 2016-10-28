#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "deviceview.h"

#include "unit_test/testMonitor.h"
#include "unit_test/testDb.h"
#include <QMessageBox>

#include <QStatusBar>
#include <QApplication>
#include <QSettings>
#include "plsetting.h"



MainWindow *g_mainWnd = NULL;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    g_mainWnd = this;
    this->setWindowTitle("PhoneLin");

    createActions();

    m_rootLayout = new QVBoxLayout();
    centralWidget()->setLayout(m_rootLayout);

    initToolBar();

    initSubView();

    initStatusBar();

    connect(m_btnSetting,SIGNAL(clicked()),this, SLOT(onSetting()));

    m_monitor_thread = new MonitorThread(this);
    m_monitor_thread->start();

}

void MainWindow::onMonitorCreated(DeviceMonitor* m)
{
    m_monitor = m;
    connect(m_monitor,SIGNAL(busyMessage(QString)),this,SLOT(onBusyMessage(QString)) );

    connect(m_monitor,SIGNAL(updateAppList(QJsonArray)),m_appView, SLOT(onDataUpdated(QJsonArray)));
    connect(m_monitor,SIGNAL(updateAppIcon(QString, QString)),m_appView, SLOT(onIconComing(QString,QString)));
    connect(m_monitor,SIGNAL(updateView(int)),this,SLOT(onUpdateView(int)));
    connect(m_monitor,SIGNAL(refreshFileList()),m_fileView,SLOT(onRefresh()));
    connect(m_monitor,SIGNAL(sdCartListComing(QJsonArray)),m_fileView,SLOT(onSdCartListComing(QJsonArray)));
    connect(m_monitor,SIGNAL(fileListUpdated(QJsonObject)),m_fileView,SLOT(onDataUpdated(QJsonObject)));
    connect(m_monitor,SIGNAL(showDeviceInfo(QJsonObject)),m_homeView,SLOT(onShowDeviceInfo(QJsonObject)));
    connect(m_monitor,SIGNAL(updateContactIcon(QString, QString)),m_contactView,SLOT(onIconComing(QString, QString)));


    connect(m_homeView, SIGNAL(netInterfaceChanged()), m_monitor, SLOT(initBroadcastAddr()));
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::onUpdateView(int idx)
{
    switch (idx)
    {
    case SMS_VIEW_IDX:
    {
        m_SmsView->onDataUpdated();
        break;
    }
    case CALL_VIEW_IDX:
    {
        m_callView->onDataUpdated();
        break;
    }
    case CONTACT_VIEW_IDX:
    {
        m_contactView->onDataUpdated();
        break;
    }

    }

}


void MainWindow::initStatusBar()
{
    QFrame *line = new QFrame(centralWidget());
    line->setObjectName(QStringLiteral("line"));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    m_rootLayout->addWidget(line);

    m_btnSetting = new QToolButton(this);
    m_btnSetting->setIcon(QIcon(":/icon/icon/setting_samll.png"));
    m_btnSetting->setStyleSheet("background: transparent;");
    m_btnSetting->setPopupMode(QToolButton::InstantPopup);
    m_btnSetting->setMenu(m_context_menu);

    statusBar()->setStyleSheet(QString("QStatusBar::item{border:0px}"));
    statusBar()->setSizeGripEnabled(false);

    statusBar()->addPermanentWidget(m_btnSetting);
}


void MainWindow::setIcon4Btn(QToolButton *btn, QString txt,QString iconPath)
{
    btn->setText(txt);
    btn->setIcon(QIcon(iconPath));
    btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
}

void MainWindow::onShowHome()
{
    changeSubView(HOME_VIEW_IDX);
}

void MainWindow::onShowSMS()
{
    changeSubView(SMS_VIEW_IDX);
}

void MainWindow::onShowCall()
{
    changeSubView(CALL_VIEW_IDX);
}

void MainWindow::onShowContact()
{
    changeSubView(CONTACT_VIEW_IDX);
}

void MainWindow::onShowApp()
{
    if (m_curDevID.isEmpty())
    {
        return;
    }

    changeSubView(APP_VIEW_IDX);
}

void MainWindow::onShowFile()
{
    if (m_curDevID.isEmpty())
    {
        return;
    }

    changeSubView(FILE_VIEW_IDX);
}


void MainWindow::changeSubView(int viewIdx)
{
    if (viewIdx != HOME_VIEW_IDX)
    {
        m_homeView->hide();
        setIcon4Btn(m_btn_home,"Home",":/head/head/sys-info-menu.png");
    }

    if (viewIdx != SMS_VIEW_IDX)
    {
        m_SmsView->hide();
        setIcon4Btn(m_btn_sms,"SMS",":/head/head/sms-thread-menu.png");
    }

    if (viewIdx != CONTACT_VIEW_IDX)
    {
        m_contactView->hide();
        setIcon4Btn(m_btn_contact,"Contact",":/head/head/contact-menu.png");
    }

    if (viewIdx != APP_VIEW_IDX)
    {
        m_appView->hide();
        setIcon4Btn(m_btn_app,"App",":/head/head/app-menu.png");
    }

    if (viewIdx != CALL_VIEW_IDX)
    {
        m_callView->hide();
        setIcon4Btn(m_btn_call,"Call",":/head/head/call-menu.png");
    }

    if (viewIdx != FILE_VIEW_IDX)
    {
        m_fileView->hide();
        setIcon4Btn(m_btn_file,"Storage",":/head/head/file-menu.png");
    }

    switch (viewIdx)
    {
    case HOME_VIEW_IDX:
    {
        m_homeView->show();
        setIcon4Btn(m_btn_home,"Home",":/head/head/sys-info-menu-sel.png");
        break;
    }
    case SMS_VIEW_IDX:
    {
        m_SmsView->show();
        setIcon4Btn(m_btn_sms,"SMS",":/head/head/sms-thread-menu-sel.png");
        break;
    }
    case CALL_VIEW_IDX:
    {
        m_callView->show();
        setIcon4Btn(m_btn_call,"Call",":/head/head/call-menu-sel.png");
        break;
    }
    case CONTACT_VIEW_IDX:
    {
        m_contactView->show();
        setIcon4Btn(m_btn_contact,"Contact",":/head/head/contact-menu-sel.png");
        break;
    }
    case APP_VIEW_IDX:
    {
        emit m_monitor->refreshAppList(m_curDevID,0);
        //DeviceMonitor::refreshAppList(m_curDevID,0);
        m_appView->show();
        setIcon4Btn(m_btn_app,"App",":/head/head/app-menu-sel.png");
        break;
    }
    case FILE_VIEW_IDX:
    {
        m_fileView->onRefresh();
        m_fileView->show();
        setIcon4Btn(m_btn_file,"Storage",":/head/head/file-menu-sel.png");
        break;
    }
    }
}



void MainWindow::initToolBar()
{
    QToolBar *pToolBar = new QToolBar();
    pToolBar->setIconSize(QSize(32,32));
    QFont font = pToolBar->font();
    font.setPointSize(PL_MAIN_BTN_FONT_SIZE);
    //pToolBar->setFont(font);
    pToolBar->layout()->setSpacing(20);

    m_btn_home = new QToolButton();
    setIcon4Btn(m_btn_home,"Home",":/head/head/sys-info-menu-sel.png");
    connect(m_btn_home, SIGNAL(clicked(bool)), this, SLOT(onShowHome()));

    m_btn_sms = new QToolButton();
    setIcon4Btn(m_btn_sms,"SMS",":/head/head/sms-thread-menu.png");
    connect(m_btn_sms, SIGNAL(clicked(bool)), this, SLOT(onShowSMS()));

    m_btn_call = new QToolButton();
    setIcon4Btn(m_btn_call,"Call",":/head/head/call-menu.png");
    connect(m_btn_call, SIGNAL(clicked(bool)), this, SLOT(onShowCall()));

    m_btn_contact = new QToolButton();
    setIcon4Btn(m_btn_contact,"Contact",":/head/head/contact-menu.png");
    connect(m_btn_contact, SIGNAL(clicked(bool)), this, SLOT(onShowContact()));

    m_btn_app = new QToolButton();
    setIcon4Btn(m_btn_app,"App",":/head/head/app-menu.png");
    connect(m_btn_app, SIGNAL(clicked(bool)), this, SLOT(onShowApp()));

    m_btn_file = new QToolButton();
    setIcon4Btn(m_btn_file,"Storage",":/head/head/file-menu.png");
    connect(m_btn_file, SIGNAL(clicked(bool)), this, SLOT(onShowFile()));

    pToolBar->addWidget(m_btn_home);
    pToolBar->addWidget(m_btn_file);
    pToolBar->addWidget(m_btn_contact);
    pToolBar->addWidget(m_btn_app);
    pToolBar->addWidget(m_btn_sms);
    pToolBar->addWidget(m_btn_call);

    //pToolBar->addWidget(btn_media);

    addToolBar(pToolBar);
}


void MainWindow::initSubView()
{
    m_homeView = new DeviceView(centralWidget());
    m_SmsView = new SmsView(centralWidget());
    m_contactView = new ContactView(centralWidget());
    m_appView = new AppView(centralWidget());
    m_callView = new CalllogView(centralWidget());
    m_fileView = new FileView(centralWidget());

    m_SmsView->hide();
    m_contactView->hide();
    m_appView->hide();
    m_callView->hide();
    m_fileView->hide();

    QVBoxLayout *subViewLayout = new QVBoxLayout();
    subViewLayout->addWidget(m_homeView);
    subViewLayout->addWidget(m_SmsView);
    subViewLayout->addWidget(m_callView);
    subViewLayout->addWidget(m_contactView);
    subViewLayout->addWidget(m_appView);
    subViewLayout->addWidget(m_fileView);

    m_rootLayout->addLayout(subViewLayout);

}


void MainWindow::onBusyMessage(QString msg)
{
    statusBar()->showMessage(msg);
}



void MainWindow::onShowErrorInfo(QString title,QString error)
{
    QMessageBox msgBox;
    //QString msg = "PhoneLin V0.1  \n";
    //msg += "Feel free to contact me at phonelin@phonelin.org";
    msgBox.setText(error);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Warning);

    msgBox.exec();
}


void MainWindow::onAbout()
{
    QMessageBox msgBox;
    QString msg = "PhoneLin V0.01 alpha \n";
    msg += "Visit <a href='http://www.phonelin.org'>www.phonelin.org</a> for more information";
    msgBox.setText(msg);
    msgBox.setTextFormat(Qt::RichText);
    //msgBox.setInformativeText( "About PhoneLin");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Information);

    msgBox.exec();
}



void MainWindow::onSetting()
{
    PlSetting *setting = new PlSetting(this);
    setting->show();
    if (!setting->Accepted)
    {
        return;
    }


}


void MainWindow::onSyncMedia()
{
    emit m_monitor->loadMediaData(this->m_curDevID);
}




void MainWindow::createActions()
{
    m_context_menu = new QMenu();
    m_about_act = new QAction(this);
    m_about_act->setText(QString("About"));
    m_context_menu->addAction(m_about_act);

    m_setting_act = new QAction(this);
    m_setting_act->setText(QString("Setting"));
    m_context_menu->addAction(m_setting_act);

    m_sync_media_act = new QAction(this);
    m_sync_media_act->setText(QString("Sync Media now"));
    m_context_menu->addAction(m_sync_media_act);

    QObject::connect(m_about_act, SIGNAL(triggered()), this, SLOT(onAbout()));
    QObject::connect(m_setting_act, SIGNAL(triggered()), this, SLOT(onSetting()));
    QObject::connect(m_sync_media_act, SIGNAL(triggered()), this, SLOT(onSyncMedia()));
} 


void MainWindow::onDeviceDisconnected(QString devid)
{
    DEBUG_INFO << "Enter";
    m_homeView->onDeviceDisconnected();
    return;

}

void MainWindow::onDeviceConnected(QString devid)
{
    DEBUG_INFO << "Enter:" << devid;
    m_curDevID = devid;

}



