#ifndef SMSCHATView_H
#define SMSCHATView_H

#include <QWidget>
#include "phonelin.h"


namespace Ui {
class SmsChatView;
}

class SmsChatView : public QWidget
{
    Q_OBJECT

public:
    explicit SmsChatView(QWidget *parent = 0);
    ~SmsChatView();

private:
    Ui::SmsChatView *ui;
};

#endif // SMSCHATView_H
