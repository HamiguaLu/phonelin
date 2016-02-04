#ifndef FILEMANAGERUI_H
#define FILEMANAGERUI_H

#include <QWidget>

namespace Ui {
class FileManagerUI;
}

class FileManagerUI : public QWidget
{
    Q_OBJECT

public:
    explicit FileManagerUI(QWidget *parent = 0);
    ~FileManagerUI();

private:
    Ui::FileManagerUI *ui;
};

#endif // FILEMANAGERUI_H
