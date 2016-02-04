#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "deviceui.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_m_btnMyPhone_clicked();

private:
    Ui::MainWindow *ui;
    deviceUI *p1;

    void initSubUi();
};

#endif // MAINWINDOW_H
