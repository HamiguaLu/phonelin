#ifndef CALLLOGUI_H
#define CALLLOGUI_H

#include <QWidget>

namespace Ui {
class CalllogUI;
}

class CalllogUI : public QWidget
{
    Q_OBJECT

public:
    explicit CalllogUI(QWidget *parent = 0);
    ~CalllogUI();

private:
    Ui::CalllogUI *ui;
};

#endif // CALLLOGUI_H
