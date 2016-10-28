#ifndef PLSETTING_H
#define PLSETTING_H

#include <QDialog>

namespace Ui {
class PlSetting;
}

class PlSetting : public QDialog
{
    Q_OBJECT

public:
    explicit PlSetting(QWidget *parent = 0);
    ~PlSetting();

private slots:
    void on_btnChooseSyncFolder_clicked();

    void on_buttonBox_accepted();

private:
    Ui::PlSetting *ui;
};

#endif // PLSETTING_H
