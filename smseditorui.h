#ifndef SMSEDITORUI_H
#define SMSEDITORUI_H

#include <QWidget>

namespace Ui {
class SmsEditorUI;
}

class SmsEditorUI : public QWidget
{
    Q_OBJECT

public:
    explicit SmsEditorUI(QWidget *parent = 0);
    ~SmsEditorUI();

private:
    Ui::SmsEditorUI *ui;
};

#endif // SMSEDITORUI_H
