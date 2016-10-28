#include "smseditorview.h"
#include "ui_smseditor.h"
#include "mainwindow.h"

extern MainWindow *g_mainWnd;
SmsEditorView::SmsEditorView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SmsEditorView)
{
    ui->setupUi(this);

    ui->btnChooser->hide();
}

SmsEditorView::~SmsEditorView()
{
    delete ui;
}


void SmsEditorView::on_btnSend_clicked()
{
    QString recipients = ui->recipients->toPlainText();
    QString content = ui->content->document()->toPlainText();

    if (recipients.isEmpty() || content.isEmpty())
    {
        return;
    }

    emit g_mainWnd->m_monitor->sendSMS(g_mainWnd->m_curDevID,recipients,content);

    close();

}
