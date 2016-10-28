#include "smschatview.h"
#include "ui_smschat.h"

SmsChatView::SmsChatView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SmsChatView)
{
    ui->setupUi(this);
}

SmsChatView::~SmsChatView()
{
    delete ui;
}
