#include "fileview.h"
#include "ui_file.h"
#include <QtCore/QDebug>

#include <QMessageBox>
#include <QJsonObject>
#include <QPushButton>
#include <QHBoxLayout>
#include "mainwindow.h"

#include "input_dialog.h"

extern MainWindow *g_mainWnd;

FileView::FileView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileView)
{
    ui->setupUi(this);
    m_layout = new QVBoxLayout();

    createActions();
    initToolBar();

    QHBoxLayout *l = new QHBoxLayout;
    m_folderPath = new QLineEdit(this);
    QToolButton *btnGo = new QToolButton(this);
    btnGo->setIcon(QIcon(":/icon/icon/next.png"));
    btnGo->setToolTip("Go");

    m_folderPath->installEventFilter(this);

    l->addWidget(m_folderPath);
    l->addWidget(btnGo);
    m_layout->addLayout(l);

    m_fileTable = new QTableView(this);
    m_layout->addWidget(m_fileTable);

    //m_fileTable->setStyleSheet("background-color:green;");
    //m_fileTable->setGridStyle(Qt::NoPen);
    m_fileTable->setContextMenuPolicy(Qt::CustomContextMenu);

    setLayout(m_layout);


    m_model  = new FileTableModel(this);

    m_delegate = new FileHtmlDelegate();

    m_fileTable->setModel(m_model);
    m_fileTable->setItemDelegate(m_delegate);

    m_fileTable->verticalHeader()->hide();
    m_fileTable->horizontalHeader()->hide();

    connect(btnGo,SIGNAL(clicked()),this, SLOT(onNext()));
    connect(m_model,SIGNAL(resizeView()),this,SLOT(onResizeTableView()));
    connect(m_fileTable, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(onDoubleClicked(const QModelIndex &)));

    onResizeTableView();

}



FileView::~FileView()
{
    delete ui;
}


void FileView::onSdCartListComing(QJsonArray arry)
{
    QJsonObject l = arry.at(0).toObject();
    m_curRootPath = l.value("path").toString();

    m_sCurrentPath = m_curRootPath;
}


void FileView::onDataUpdated(QJsonObject fileData)
{
    QJsonArray array = fileData.value("data").toArray();

    m_sCurrentPath = fileData.value("path").toString();
    m_folderPath->setText(m_sCurrentPath);

    QJsonArray *ary = new QJsonArray(array);

    m_model->setData(ary);

    onResizeTableView();

    repaint();
}



void FileView::onResizeTableView()
{  
    if (m_model == NULL || m_fileTable->model() == NULL)
    {
        return;
    }

    for (int i = 0; i < m_model->m_itemsPerRow; ++i)
    {
        m_fileTable->setColumnWidth(i,FILE_VIEW_WIDTH);
    }

    for (int i = 0; i < m_fileTable->model()->rowCount(); ++i)
    {
        m_fileTable->setRowHeight(i,FILE_VIEW_HEIGHT);
    }

}


void FileView::resizeEvent(QResizeEvent* evt)
{
    int w = evt->size().width();

    if (m_model != NULL)
    {
        m_model->setItemCountPerRow(w/FILE_VIEW_WIDTH);
    }
}



void FileView::createActions()
{
    m_context_menu = new QMenu();
    m_rm_file_act = new QAction(this);
    m_rm_file_act->setText(QString("Remove"));
    QObject::connect(m_rm_file_act, SIGNAL(triggered()), this, SLOT(onRemoveFile()));

    m_rename_file_act = new QAction(this);
    m_rename_file_act->setText(QString("Rename"));
    QObject::connect(m_rename_file_act, SIGNAL(triggered()), this, SLOT(onRenameFile()));

    m_download_file_act = new QAction(this);
    m_download_file_act->setText(QString("Download"));
    QObject::connect(m_download_file_act, SIGNAL(triggered()), this, SLOT(onDownloadFile()));

    m_upload_file_act = new QAction(this);
    m_upload_file_act->setText(QString("Upload"));
    QObject::connect(m_upload_file_act, SIGNAL(triggered()), this, SLOT(onUploadFile()));
}


void FileView::initToolBar()
{
    QToolBar *pToolBar = new QToolBar(this);
    pToolBar->layout()->setSpacing(20);

    m_layout->setMenuBar(pToolBar);
    QToolButton* toolBtnNew = new QToolButton(pToolBar);
    toolBtnNew->setIcon(QIcon(":/file_type/file_icon/file_add.png"));
    toolBtnNew->setToolTip("New folder");
    pToolBar->addWidget(toolBtnNew);

    pToolBar->addSeparator();

    QToolButton* toolBtnCopy = new QToolButton(pToolBar);
    toolBtnCopy->setIcon(QIcon(":/file_type/file_icon/file_copy.png"));
    toolBtnCopy->setToolTip("Copy file");
    pToolBar->addWidget(toolBtnCopy);
    //toolBtnCopy->setEnabled(false);

    QToolButton* toolBtnCut = new QToolButton(pToolBar);
    toolBtnCut->setIcon(QIcon(":/file_type/file_icon/file_cut.png"));
    toolBtnCut->setToolTip("Cut file");
    pToolBar->addWidget(toolBtnCut);

    QToolButton* toolBtnPaste = new QToolButton(pToolBar);
    toolBtnPaste->setIcon(QIcon(":/file_type/file_icon/file_paste.png"));
    toolBtnPaste->setToolTip("Paste file");
    pToolBar->addWidget(toolBtnPaste);

    pToolBar->addSeparator();

    QToolButton* toolBtnRename = new QToolButton(pToolBar);
    toolBtnRename->setIcon(QIcon(":/file_type/file_icon/file_rename.png"));
    toolBtnRename->setToolTip("Rename");
    pToolBar->addWidget(toolBtnRename);

    QToolButton* toolBtnDel = new QToolButton(pToolBar);
    toolBtnDel->setIcon(QIcon(":/file_type/file_icon/file_del.png"));
    toolBtnDel->setToolTip("Delete");
    pToolBar->addWidget(toolBtnDel);

    pToolBar->addSeparator();

    QToolButton* toolBtnDownload = new QToolButton(pToolBar);
    toolBtnDownload->setIcon(QIcon(":/file_type/file_icon/file_download.png"));
    toolBtnDownload->setToolTip("Download");
    pToolBar->addWidget(toolBtnDownload);

    QToolButton* toolBtnUpload = new QToolButton(pToolBar);
    toolBtnUpload->setIcon(QIcon(":/file_type/file_icon/file_upload.png"));
    toolBtnUpload->setToolTip("Upload");
    pToolBar->addWidget(toolBtnUpload);

    pToolBar->addSeparator();

    QToolButton* toolBtnBack = new QToolButton(pToolBar);
    toolBtnBack->setIcon(QIcon(":/icon/icon/pre.png"));
    toolBtnBack->setToolTip("Back");
    pToolBar->addWidget(toolBtnBack);

#if 0
    QToolButton* toolBtnNext = new QToolButton(pToolBar);
    toolBtnNext->setIcon(QIcon(":/icon/icon/next.png"));
    toolBtnNext->setToolTip("Next");
    pToolBar->addWidget(toolBtnNext);
#endif

    QToolButton* toolBtnRefresh = new QToolButton(pToolBar);
    toolBtnRefresh->setIcon(QIcon(":/file_type/file_icon/file_refresh.png"));
    toolBtnRefresh->setToolTip("Refresh");
    pToolBar->addWidget(toolBtnRefresh);

    connect(toolBtnDel, SIGNAL(clicked()), this, SLOT(onRemoveFile()));
    connect(toolBtnNew, SIGNAL(clicked()), this, SLOT(onNewFile()));
    connect(toolBtnRename, SIGNAL(clicked()), this, SLOT(onRenameFile()));
    connect(toolBtnDownload, SIGNAL(clicked()), this, SLOT(onDownloadFile()));
    connect(toolBtnUpload, SIGNAL(clicked()), this, SLOT(onUploadFile()));

    connect(toolBtnCopy, SIGNAL(clicked()), this, SLOT(onCopyFile()));
    connect(toolBtnCut, SIGNAL(clicked()), this, SLOT(onCutFile()));
    connect(toolBtnPaste, SIGNAL(clicked()), this, SLOT(onPasteFile()));

    connect(toolBtnBack, SIGNAL(clicked()), this, SLOT(onBack()));
    //connect(toolBtnNext, SIGNAL(clicked()), this, SLOT(onNext()));
    connect(toolBtnRefresh, SIGNAL(clicked()), this, SLOT(onRefresh()));

}


bool FileView::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Return)
        {
            onNext();
        }
    }

    // standard event processing
    return QObject::eventFilter(obj, event);
}



void FileView::onDoubleClicked(const QModelIndex & index)
{
    QJsonObject app = m_model->data(index,Qt::UserRole).toJsonObject();
    QString path = m_sCurrentPath + "/" + app.value("name").toString();
    path = path.replace("//","/");
    bool isDir = app.value("isDir").toBool();
    if (isDir)
    {
        if (m_history.size() > 1000)
        {
            m_history.clear();
        }

        m_history.push(path);

        emit g_mainWnd->m_monitor->getFileList(g_mainWnd->m_curDevID,path,0);
        return;
    }

    onDownloadFile();

}



void FileView::onNewFile()
{
    InputDialog dlg(this);
    dlg.exec();

    QString newName = dlg.m_newFileName;
    if (newName.isEmpty())
    {
        return;
    }

    QString path = util_make_path(m_sCurrentPath , newName);
    emit g_mainWnd->m_monitor->newFile(g_mainWnd->m_curDevID, path,true);
}


void FileView::onRenameFile()
{
    QModelIndexList indexList = m_fileTable->selectionModel()->selectedIndexes();
    if (indexList.count() != 1)
    {
        return;
    }

    QJsonObject app = m_model->data(indexList.at(0),Qt::UserRole).toJsonObject();
    QString name = app.value("name").toString();
    if (name.isEmpty())
    {
        return;
    }

    QString info = "Rename " + name + " to:";

    InputDialog dlg(this);
    dlg.setInfo(info);
    dlg.setTitle("Rename File");
    dlg.exec();

    QString newName = dlg.m_newFileName;
    if (newName.isEmpty())
    {
        return;
    }

    QString srcPath = util_make_path(m_sCurrentPath , name);
    QString path = util_make_path(m_sCurrentPath , newName);

    emit g_mainWnd->m_monitor->renameFile(g_mainWnd->m_curDevID, srcPath, path);


}

void FileView::onDownloadFile()
{
    QModelIndexList indexList = m_fileTable->selectionModel()->selectedIndexes();
    if (indexList.count() != 1)
    {
        return;
    }

    QJsonObject app = m_model->data(indexList.at(0),Qt::UserRole).toJsonObject();
    QString name = app.value("name").toString();
    if (name.isEmpty())
    {
        return;
    }

    QString localPath = util_make_path(QDir::tempPath(), name);

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save File"),
                                                    localPath);
    if(fileName.isEmpty())
    {
        QMessageBox::information(this,"Info","Please choose a file");
        return;
    }


    QString path = util_make_path(m_sCurrentPath , name);


    emit g_mainWnd->m_monitor->downloadFile(g_mainWnd->m_curDevID, path, fileName);
}

void FileView::onUploadFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,"Open file",QDir::homePath(),"All Files (*.*)");
    if(fileName.isEmpty())
    {
        QMessageBox::information(this,"Info","Please choose a file");
        return;
    }

    QFileInfo fi;
    fi = QFileInfo(fileName);
    QString dstFile = fi.fileName();

    QString dstPath = util_make_path(m_sCurrentPath , dstFile);

    emit g_mainWnd->m_monitor->uploadFile(g_mainWnd->m_curDevID,fileName, dstPath);

}

void FileView::onRemoveFile()
{
    QModelIndexList indexList = m_fileTable->selectionModel()->selectedIndexes();
    if (indexList.count() != 1)
    {
        return;
    }

    QJsonObject app = m_model->data(indexList.at(0),Qt::UserRole).toJsonObject();
    QString name = app.value("name").toString();
    if (name.isEmpty())
    {
        return;
    }

    QMessageBox msgBox;
    //msgBox.setText("Are you sure?");
    QString info = QString("Are you sure to remove %1 on your phone?").arg(name);
    msgBox.setInformativeText(info);
    msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::Cancel);

    if (msgBox.exec() == QMessageBox::Yes)
    {
        QString srcPath = util_make_path(m_sCurrentPath , name);
        emit g_mainWnd->m_monitor->removeFile(g_mainWnd->m_curDevID,srcPath);
    }
}



void FileView::onCopyFile()
{
    QModelIndexList indexList = m_fileTable->selectionModel()->selectedIndexes();
    if (indexList.count() != 1)
    {
        return;
    }

    QJsonObject app = m_model->data(indexList.at(0),Qt::UserRole).toJsonObject();
    QString name = app.value("name").toString();
    if (name.length() < 1)
    {
        return;
    }

    m_srcFilePath = util_make_path(m_sCurrentPath , name);

    m_ifileOpMode = FILE_OP_COPY;
}

void FileView::onCutFile()
{
    QModelIndexList indexList = m_fileTable->selectionModel()->selectedIndexes();
    if (indexList.count() != 1)
    {
        return;
    }

    QJsonObject app = m_model->data(indexList.at(0),Qt::UserRole).toJsonObject();
    QString name = app.value("name").toString();
    if (name.length() < 1)
    {
        return;
    }

    m_srcFilePath = util_make_path(m_sCurrentPath , name);
    m_ifileOpMode = FILE_OP_CUT;

}

void FileView::onPasteFile()
{
    QFileInfo fi;
    fi = QFileInfo(m_srcFilePath);
    QString dstFile = fi.fileName();

    if (dstFile.isEmpty())
    {
        return;
    }

    if (m_ifileOpMode == FILE_OP_COPY)
    {
        if (dstFile == m_srcFilePath)
        {
            dstFile = "new_" + dstFile;
        }
    }

    dstFile = util_make_path(m_sCurrentPath , dstFile);

    if (m_ifileOpMode == FILE_OP_CUT)
    {
        if (dstFile != m_srcFilePath)
        {
            emit g_mainWnd->m_monitor->copyFile(g_mainWnd->m_curDevID,FILE_OP_CUT, m_srcFilePath,dstFile);
        }
    }
    else if (m_ifileOpMode == FILE_OP_COPY)
    {
        if (dstFile == m_srcFilePath)
        {
            dstFile = util_make_path(m_sCurrentPath , "new_" + fi.fileName());
        }

        emit g_mainWnd->m_monitor->copyFile(g_mainWnd->m_curDevID,FILE_OP_COPY, m_srcFilePath,dstFile);
    }


}

void FileView::onBack()
{
#if 0
    if (m_history.size() < 1)
    {
        return;
    }

    QString path = m_history.pop();
#endif

    QString path = m_folderPath->text();
    int end = path.lastIndexOf("/");
    path = path.left(end);
    if (path.isEmpty() || !path.contains(m_curRootPath))
    {
        path = m_curRootPath;
    }


    emit g_mainWnd->m_monitor->getFileList(g_mainWnd->m_curDevID,path,0);
}


void FileView::onNext()
{
    QString path = m_folderPath->text();
    if (path.isEmpty() || !path.contains(m_curRootPath))
    {
        return;
    }

    emit g_mainWnd->m_monitor->getFileList(g_mainWnd->m_curDevID,path,0);


}


void FileView::onRefresh()
{
    if (m_curRootPath.isEmpty())
    {
        return;
    }

    if (m_sCurrentPath.isEmpty() || !m_sCurrentPath.contains(m_curRootPath))
    {
        m_sCurrentPath = m_curRootPath;
    }

    emit g_mainWnd->m_monitor->getFileList(g_mainWnd->m_curDevID,m_sCurrentPath,0);

}




void FileView::onContextMenuEvent(const QPoint &point)
{
    m_context_menu->clear();
    
    QModelIndex index = m_fileTable->indexAt(point);
    QJsonObject app = m_model->data(index,Qt::UserRole).toJsonObject();

    if(app.contains("name"))
    {
        m_rm_file_act->setData(app);
        m_context_menu->addAction(m_rm_file_act);
        m_context_menu->exec(QCursor::pos());
    }

}




