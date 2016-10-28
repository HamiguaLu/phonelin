
#include "calllogview.h"
#include "ui_calllog.h"
#include <QVBoxLayout>
#include "db/dbHelper.h"


CalllogView::CalllogView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalllogView)
{
    ui->setupUi(this);

	QVBoxLayout *layout = new QVBoxLayout();

	m_pagger = new PageWidget();
    connect(m_pagger, SIGNAL(currentPageChanged(int)), this, SLOT(onPageChanged(int)));
    layout->addWidget(m_pagger);

    m_callLogView = new QTableView(this);
    m_callLogView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_callLogView->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); 

    layout->addWidget(m_callLogView);

    this->setLayout(layout);

	m_model  = new CallLogModel(this);
    m_callLogView->setModel(m_model);

    onDataUpdated();

    m_callLogView->verticalHeader()->hide();
    m_callLogView->setColumnHidden(0,true);

}

CalllogView::~CalllogView()
{
    delete ui;
}


void CalllogView::onPageChanged(int page)
{
    m_curPage = page;
	onDataUpdated();
}


void CalllogView::onDataUpdated()
{
    QJsonArray *data = new QJsonArray();
    if (PL_RET_OK != DbHelper::loadCallList((m_curPage - 1) * MAX_CALL_PER_PAGE,MAX_CALL_PER_PAGE,(*data)))
    {
    	return;
    }

	m_model->setData(data);

	int count = DbHelper::getCallCount();
	int maxPage = count/MAX_CALL_PER_PAGE + ((count % MAX_CALL_PER_PAGE > 0) ? 1:0);
    m_pagger->setMaxPage(maxPage);
    m_pagger->setCurrentPage(m_curPage);
        
    m_callLogView->resizeColumnsToContents();
    m_callLogView->resizeRowsToContents();
    repaint();
}







