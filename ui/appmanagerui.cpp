#include "appmanagerui.h"
#include "ui_appmanagerui.h"

AppManagerUI::AppManagerUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AppManagerUI)
{
    ui->setupUi(this);
}

AppManagerUI::~AppManagerUI()
{
    delete ui;
}
