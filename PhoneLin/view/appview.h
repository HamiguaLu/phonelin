#ifndef APPMANAGERView_H
#define APPMANAGERView_H

#include <QWidget>
#include <QTableView>
#include <QJsonArray>

#include "appHtmlDelegate.h"
#include "appTableModel.h"

#include "../3rdpart/PageWidget.h"
#include "phonelin.h"

#include <QResizeEvent>
#include <QMenu>
#include <QAction>
#include <QVBoxLayout>

namespace Ui {
class AppView;
}

class AppView : public QWidget
{
    Q_OBJECT

public:
    explicit AppView(QWidget *parent = 0);
    ~AppView();

private:
    Ui::AppView *ui;
    QVBoxLayout *m_layout;

    QTableView *m_appTable = NULL;
    AppTableModel *m_model = NULL;
    AppHtmlDelegate *m_delegate = NULL;

    PageWidget *m_pagger;

private Q_SLOTS:
    void onResizeTableView();

public slots:
    void onDoubleClicked(const QModelIndex & index);
    void onDataUpdated(QJsonArray appAry);
    void onIconComing(QString packageName,QString iconPath);
    void resizeEvent(QResizeEvent* evt);
    void onContextMenuEvent(const QPoint &point);

    void onRemoveApp();
    void onInstallApk();
    void onRefreshAppList();

private:
    const int APP_VIEW_WIDTH = 145;
    const int APP_VIEW_HEIGHT = 120;

    QMenu *m_context_menu;
    QAction *m_rm_app_act;

    void createActions();
    void initToolBar();

};

#endif // APPMANAGERView_H
