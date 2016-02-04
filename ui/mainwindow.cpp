#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QVBoxLayout"
#include "deviceui.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initSubUi();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_m_btnMyPhone_clicked()
{

}

void MainWindow::initSubUi()
{
   QLayout* p =  (QLayout*)layout();

   p1 = new deviceUI(this);

   ui->verticalLayout->addWidget(p1);



}
