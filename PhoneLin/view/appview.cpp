
#include "appview.h"
#include "ui_app.h"
#include <QtCore/QDebug>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QJsonObject>
#include <QToolBar>
#include <QToolButton>
#include <QFileDialog>

#include "mainwindow.h"


extern MainWindow *g_mainWnd;

AppView::AppView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AppView)
{
    ui->setupUi(this);
    m_layout = new QVBoxLayout();

    createActions();
	initToolBar();

    m_appTable = new QTableView(this);
    m_layout->addWidget(m_appTable);

    m_appTable->setStyleSheet("background-color:white;");
    //m_appTable->setContextMenuPolicy(Qt::CustomContextMenu);

    setLayout(m_layout);

    m_model  = new AppTableModel(this);

    m_delegate = new AppHtmlDelegate();

    m_appTable->setModel(m_model);
    m_appTable->setItemDelegate(m_delegate);

    m_appTable->verticalHeader()->hide();
    m_appTable->horizontalHeader()->hide();

    //connect(m_appTable,SIGNAL(customContextMenuRequested(const QPoint &)),this, SLOT(onContextMenuEvent(const QPoint &)));
    connect(m_model,SIGNAL(resizeView()),this,SLOT(onResizeTableView()));
    //connect(m_appTable, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(onDoubleClicked(const QModelIndex &)));

    onResizeTableView();

}

AppView::~AppView()
{
    delete ui;
}


void AppView::onDataUpdated(QJsonArray appAry)
{
    QJsonArray *ary = new QJsonArray(appAry);

    m_model->setData(ary);

    onResizeTableView();

    repaint();
}

void AppView::onIconComing(QString packageName, QString iconPath)
{
	if (m_model == NULL || m_appTable->model() == NULL)
	{
		return;
	}

	m_model->setIcon(packageName,iconPath);
}


void AppView::onResizeTableView()
{
    if (m_model == NULL || m_appTable->model() == NULL)
    {
        return;
    }

    for (int i = 0; i < m_model->m_itemsPerRow; ++i)
    {
        m_appTable->setColumnWidth(i,APP_VIEW_WIDTH);
    }

    for (int i = 0; i < m_appTable->model()->rowCount(); ++i)
    {
        m_appTable->setRowHeight(i,APP_VIEW_HEIGHT);
    }
}


void AppView::resizeEvent(QResizeEvent* evt)
{
    int w = evt->size().width();

    if (m_model != NULL)
    {
        m_model->setItemCountPerRow(w/APP_VIEW_WIDTH);
    }

}

void AppView::onDoubleClicked(const QModelIndex & index)
{
    QJsonObject app = m_model->data(index,Qt::UserRole).toJsonObject();

    /*QMessageBox msgBox;
    msgBox.setText("Name");
    msgBox.setInformativeText( app.value("appname").toString());
    msgBox.setStandardButtons(QMessageBox::Ok);

    msgBox.exec();*/
}


void AppView::createActions()
{
    m_context_menu = new QMenu();
    m_rm_app_act = new QAction(this);
    m_rm_app_act->setText(QString("Remove"));
    QObject::connect(m_rm_app_act, SIGNAL(triggered()), this, SLOT(onRemoveApp()));
}  


void AppView::initToolBar()
{
    QToolBar *pToolBar = new QToolBar(this);
    pToolBar->layout()->setSpacing(20);

    m_layout->setMenuBar(pToolBar);

    QToolButton* toolBtnInstall = new QToolButton(pToolBar);
    toolBtnInstall->setIcon(QIcon(":/icon/icon/install_app.png"));
    toolBtnInstall->setToolTip("Install Apk");
    pToolBar->addWidget(toolBtnInstall);
	connect(toolBtnInstall, SIGNAL(clicked(bool)), this, SLOT(onInstallApk()));

    QToolButton* toolBtnRemove = new QToolButton(pToolBar);
    toolBtnRemove->setIcon(QIcon(":/icon/icon/uninstall_app.png"));
    toolBtnRemove->setToolTip("Remove App");
    pToolBar->addWidget(toolBtnRemove);
	connect(toolBtnRemove, SIGNAL(clicked(bool)), this, SLOT(onRemoveApp()));

    QToolButton* toolBtnRefresh = new QToolButton(pToolBar);
    toolBtnRefresh->setIcon(QIcon(":/icon/icon/refresh_app.png"));
    toolBtnRefresh->setToolTip("Refresh App List");
    pToolBar->addWidget(toolBtnRefresh);
	connect(toolBtnRefresh, SIGNAL(clicked(bool)), this, SLOT(onRefreshAppList()));	
}



void AppView::onRefreshAppList()
{
    emit g_mainWnd->m_monitor->refreshAppList(g_mainWnd->m_curDevID,0);
}

void AppView::onInstallApk()
{
	QString fileName = QFileDialog::getOpenFileName(this,"Open APK file",QDir::homePath(),"Android Apk Files (*.apk)");
	if(fileName.isEmpty())
	{
    	QMessageBox::information(this,"Info","Please choose a apk file");
    	return;
	}

    emit g_mainWnd->m_monitor->installApp(g_mainWnd->m_curDevID,fileName);
}

void AppView::onRemoveApp()
{
    QModelIndexList indexList = m_appTable->selectionModel()->selectedIndexes();
    if (indexList.count() != 1)
    {
        return;
    }

    QJsonObject app = m_model->data(indexList.at(0),Qt::UserRole).toJsonObject();
    QString appname = app.value("appname").toString();
    if (appname.isEmpty())
    {
        return;
    }

    QMessageBox msgBox;
    //msgBox.setText("Are you sure?");
    QString info = QString("Are you sure to remove %1 on your phone?").arg(appname);
    msgBox.setInformativeText(info);
    msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::Cancel);

    if (QMessageBox::Yes == msgBox.exec())
    {
        QString pkgName = app.value("packageName").toString();
        emit g_mainWnd->m_monitor->rmApp(g_mainWnd->m_curDevID,pkgName);
    }
}


void AppView::onContextMenuEvent(const QPoint &point)
{
    m_context_menu->clear(); //清除原有菜单
    
    QModelIndex index = m_appTable->indexAt(point);
    QJsonObject app = m_model->data(index,Qt::UserRole).toJsonObject();

    if(app.contains("appname"))
    {
        m_rm_app_act->setData(app);
        m_context_menu->addAction(m_rm_app_act);
        m_context_menu->exec(QCursor::pos());
    }

}




