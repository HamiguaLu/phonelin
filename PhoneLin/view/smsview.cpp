
#include "smsview.h"
#include "ui_sms.h"
#include "db/dbHelper.h"
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QTimer>
#include <QToolButton>
#include "smseditorview.h"

SmsView::SmsView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SmsView)
{
    ui->setupUi(this);

    QVBoxLayout *layout = new QVBoxLayout();

    m_pagger = new PageWidget(3,this);
    connect(m_pagger, SIGNAL(currentPageChanged(int)), this, SLOT(onPageChanged(int)));
    layout->addWidget(m_pagger);

    m_smsView = new QTableView(this);
    m_smsView->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
    layout->addWidget(m_smsView);

    QToolButton *btnGo = new QToolButton(this);
    btnGo->setText("Send SMS");
    btnGo->setIcon(QIcon(":/icon/icon/send_sms.png"));
    btnGo->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnGo->setIconSize(QSize(32, 32));
    layout->addWidget(btnGo);

    connect(btnGo,SIGNAL(clicked()),this, SLOT(onSendSMS()));

    setLayout(layout);

    m_model  = new SmsTableModel(this);

    m_smsView->setModel(m_model);
    m_smsView->verticalHeader()->hide();
    m_smsView->setColumnHidden(0,true);
    m_smsView->setColumnHidden(5,true);

    onDataUpdated();

}

SmsView::~SmsView()
{
    delete ui;
}


void SmsView::onPageChanged(int page)
{
    m_curPage = page;
    onDataUpdated();
}

void SmsView::onDataUpdated()
{
    QJsonArray *data = new QJsonArray();
    if (PL_RET_OK != DbHelper::loadSMSList((m_curPage - 1) * MAX_SMS_PER_PAGE ,MAX_SMS_PER_PAGE,(*data)))
    {
        return;
    }

    m_model->setData(data);

    int count = DbHelper::getSMSCount();
    int maxPage = count/MAX_SMS_PER_PAGE + ((count % MAX_SMS_PER_PAGE > 0) ? 1:0);
    m_pagger->setMaxPage(maxPage);
    m_pagger->setCurrentPage(m_curPage);

    m_smsView->resizeRowsToContents();

    repaint();

}



void SmsView::resizeEvent(QResizeEvent* evt)
{
    int w = evt->size().width();

    m_smsView->setColumnWidth(1,w * 0.2);
    m_smsView->setColumnWidth(2,w * 0.15);
    m_smsView->setColumnWidth(3,w * 0.15);
    m_smsView->setColumnWidth(4,w * 0.4);
    m_smsView->resizeRowsToContents();
}


void SmsView::onSendSMS()
{
    SmsEditorView *editor = new SmsEditorView(this);
    //editor->setWindowModality(Qt::WindowModal);
    editor->setWindowFlags(Qt::Dialog);
    editor->show();
}



