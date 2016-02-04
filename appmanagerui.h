#ifndef APPMANAGERUI_H
#define APPMANAGERUI_H

#include <QWidget>

namespace Ui {
class AppManagerUI;
}

class AppManagerUI : public QWidget
{
    Q_OBJECT

public:
    explicit AppManagerUI(QWidget *parent = 0);
    ~AppManagerUI();

private:
    Ui::AppManagerUI *ui;
};

#endif // APPMANAGERUI_H
