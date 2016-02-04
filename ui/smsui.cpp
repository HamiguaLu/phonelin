#include "smsui.h"
#include "ui_smsui.h"

smsUI::smsUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::smsUI)
{
    ui->setupUi(this);
}

smsUI::~smsUI()
{
    delete ui;
}
