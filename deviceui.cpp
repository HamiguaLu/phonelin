#include "deviceui.h"
#include "ui_deviceui.h"

deviceUI::deviceUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::deviceUI)
{
    ui->setupUi(this);
}

deviceUI::~deviceUI()
{
    delete ui;
}
