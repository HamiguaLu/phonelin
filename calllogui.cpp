#include "calllogui.h"
#include "ui_calllogui.h"

CalllogUI::CalllogUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalllogUI)
{
    ui->setupUi(this);
}

CalllogUI::~CalllogUI()
{
    delete ui;
}
