#ifndef INPUT_DIALOG_H
#define INPUT_DIALOG_H                                                                             

#include <QVBoxLayout>
#include <QVBoxLayout>
#include <QDialog>

#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>


class InputDialog:public QDialog
{
    Q_OBJECT

public:
    InputDialog(QWidget *parent =0);
	void setInfo(QString info);
	void setTitle(QString title);
signals:
    void findNext(const QString &str,Qt::CaseSensitivity cs);
    void findPrevious(const QString &str,Qt::CaseSensitivity cs);
private slots:
    void findClicked();void enableFindButton(const QString &text);
private:
    QLabel *label;
    QLineEdit *lineEdit;
    QCheckBox *caseCheckBox;
    QCheckBox *backwardCheckBox;
    QPushButton *findButton;
    QPushButton *closeButton;

public:
	QString m_newFileName;

};                                                                               
#endif
