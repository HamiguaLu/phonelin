#ifndef CONTACTView_H
#define CONTACTView_H

#include <QWidget>
#include <QPainter>
#include <QTableView>
#include "contactHtmlDelegate.h"
#include "contactTableModel.h"

#include "../3rdpart/PageWidget.h"
#include "phonelin.h"

#include <QResizeEvent>
#include <QMenu>
#include <QAction>

namespace Ui {
class ContactView;
}

class ContactView : public QWidget
{
    Q_OBJECT

public:
    explicit ContactView(QWidget *parent = 0);
    ~ContactView();

	

private:
    Ui::ContactView *ui;

    QTableView *m_contactList = NULL;
    ContactTableModel *m_model = NULL;
    ContactHtmlDelegate *m_delegate = NULL;

    PageWidget *m_pagger;
    const int MAX_CONTACT_PER_PAGE	=	30;
    const int CONTACT_VIEW_WIDTH = 145;
    const int CONTACT_VIEW_HEIGHT = 120;

	int m_curPage = 1;
	
private Q_SLOTS:
    void onResizeTableView();	
	
public slots:
	void onDoubleClicked(const QModelIndex & index);
    void onPageChanged(int page);
	void onDataUpdated();
	void onIconComing(QString contactID, QString iconPath);

	void resizeEvent(QResizeEvent* evt);

protected:
    void paintEvent(QPaintEvent *);
};

#endif // CONTACTView_H
