#ifndef DEVICEUI_H
#define DEVICEUI_H

#include <QWidget>

namespace Ui {
class deviceUI;
}

class deviceUI : public QWidget
{
    Q_OBJECT

public:
    explicit deviceUI(QWidget *parent = 0);
    ~deviceUI();

private:
    Ui::deviceUI *ui;
};

#endif // DEVICEUI_H
