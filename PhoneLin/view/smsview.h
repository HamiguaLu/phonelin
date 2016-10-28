#ifndef SMSView_H
#define SMSView_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QTableView>
#include "smsmodel.h"
#include "../3rdpart/PageWidget.h"
#include "phonelin.h"
#include <QResizeEvent>




namespace Ui {
class SmsView;
}

class SmsView : public QWidget
{
    Q_OBJECT

public:
    explicit SmsView(QWidget *parent = 0);
    ~SmsView();
    QSqlQueryModel *m_ThreadModel = NULL;
private:
    Ui::SmsView *ui;

    QTableView *m_smsView = NULL;
    SmsTableModel *m_model = NULL;
    PageWidget *m_pagger;

    const int MAX_SMS_PER_PAGE = 30;

	int m_curPage = 1;

public slots:
    void onPageChanged(int page);
	void onDataUpdated();

    void onSendSMS();
	
    void resizeEvent(QResizeEvent* evt);


};

#endif // SMSView_H
