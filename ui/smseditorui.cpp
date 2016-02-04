#include "smseditorui.h"
#include "ui_smseditorui.h"

SmsEditorUI::SmsEditorUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SmsEditorUI)
{
    ui->setupUi(this);
}

SmsEditorUI::~SmsEditorUI()
{
    delete ui;
}
