#ifndef SMSUI_H
#define SMSUI_H

#include <QWidget>

namespace Ui {
class smsUI;
}

class smsUI : public QWidget
{
    Q_OBJECT

public:
    explicit smsUI(QWidget *parent = 0);
    ~smsUI();

private:
    Ui::smsUI *ui;
};

#endif // SMSUI_H
