#include "filemanagerui.h"
#include "ui_filemanagerui.h"

FileManagerUI::FileManagerUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileManagerUI)
{
    ui->setupUi(this);
}

FileManagerUI::~FileManagerUI()
{
    delete ui;
}
