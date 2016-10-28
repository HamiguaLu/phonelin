

#include "contactview.h"
#include "ui_contact.h"
#include "db/dbHelper.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QJsonObject>


ContactView::ContactView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContactView)
{
    ui->setupUi(this);

    //setStyleSheet("background-color: green");

    QVBoxLayout *layout = new QVBoxLayout();

	m_pagger = new PageWidget();
	connect(m_pagger, SIGNAL(currentPageChanged(int)), this, SLOT(onPageChanged(int)));	
    layout->addWidget(m_pagger);

    m_contactList = new QTableView(this);
    layout->addWidget(m_contactList);

    this->setLayout(layout);


	m_model  = new ContactTableModel(this);

	m_delegate = new ContactHtmlDelegate();

    m_contactList->setModel(m_model);
    m_contactList->setItemDelegate(m_delegate);

    m_contactList->verticalHeader()->hide();
    m_contactList->horizontalHeader()->hide();


    connect(m_model,SIGNAL(resizeView()),this,SLOT(onResizeTableView()));
    connect(m_contactList, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(onDoubleClicked(const QModelIndex &)));

    onDataUpdated();

}


void ContactView::paintEvent(QPaintEvent *)
{
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

ContactView::~ContactView()
{
    delete ui;
}




void ContactView::onResizeTableView()
{
    if (m_model == NULL || m_contactList->model() == NULL)
    {
        return;
    }

    for (int i = 0; i < m_model->m_itemsPerRow; ++i)
    {
        m_contactList->setColumnWidth(i,CONTACT_VIEW_WIDTH);
    }

    for (int i = 0; i < m_contactList->model()->rowCount(); ++i)
    {
        m_contactList->setRowHeight(i,CONTACT_VIEW_HEIGHT);
    }

}


void ContactView::onPageChanged(int page)
{
    m_curPage = page;
	onDataUpdated();
}



void ContactView::onDoubleClicked(const QModelIndex & index)
{
    QJsonObject app = m_model->data(index,Qt::UserRole).toJsonObject();

    QMessageBox msgBox;
    msgBox.setText("Name");
    msgBox.setInformativeText( app.value("appname").toString());
    msgBox.setStandardButtons(QMessageBox::Ok);

    msgBox.exec();
}


void ContactView::onDataUpdated()
{
    QJsonArray *data = new QJsonArray();
    if (PL_RET_OK != DbHelper::loadContactList((m_curPage - 1) * MAX_CONTACT_PER_PAGE ,MAX_CONTACT_PER_PAGE,(*data)))
    {
    	return;
    }

	m_model->setData(data);

	int count = DbHelper::getContactCount();
    int maxPage = count/MAX_CONTACT_PER_PAGE + ((count % MAX_CONTACT_PER_PAGE > 0) ? 1:0);
    m_pagger->setMaxPage(maxPage);

    m_pagger->setCurrentPage(m_curPage);

    onResizeTableView();

    repaint();
}

void ContactView::onIconComing(QString contactID, QString iconPath)
{
    if (m_model == NULL)
	{
		return;
	}

    m_model->resizeView();

}



void ContactView::resizeEvent(QResizeEvent* evt)
{
    int w = evt->size().width();

    if (m_model != NULL)
    {
       m_model->setItemCountPerRow(w/CONTACT_VIEW_WIDTH);
    }

}




