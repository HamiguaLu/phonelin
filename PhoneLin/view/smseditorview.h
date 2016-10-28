#ifndef SMSEDITORView_H
#define SMSEDITORView_H

#include <QWidget>
#include "phonelin.h"


namespace Ui {
class SmsEditorView;
}

class SmsEditorView : public QWidget
{
    Q_OBJECT

public:
    explicit SmsEditorView(QWidget *parent = 0);
    ~SmsEditorView();

private:
    Ui::SmsEditorView *ui;



private slots:
    void on_btnSend_clicked();
};

#endif // SMSEDITORView_H
