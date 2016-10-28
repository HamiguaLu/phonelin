#ifndef CALLLOGView_H
#define CALLLOGView_H

#include <QWidget>
#include <QTableView>
#include "calllogModel.h"
#include "../3rdpart/PageWidget.h"
#include "phonelin.h"

namespace Ui {
class CalllogView;
}



class CalllogView : public QWidget
{
    Q_OBJECT

public:
    explicit CalllogView(QWidget *parent = 0);
    ~CalllogView();

private:
    Ui::CalllogView *ui;

    QTableView *m_callLogView = NULL;
    CallLogModel *m_model = NULL;
    PageWidget *m_pagger;

    const int MAX_CALL_PER_PAGE = 30;

	int m_curPage = 1;
public slots:
    void onPageChanged(int page);
	void onDataUpdated();
};

#endif // CALLLOGView_H
