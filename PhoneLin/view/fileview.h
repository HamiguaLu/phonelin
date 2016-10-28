#ifndef FILEMANAGERView_H
#define FILEMANAGERView_H

#include <QWidget>
#include <QTableView>
#include <QJsonArray>

#include "fileHtmlDelegate.h"
#include "fileTableModel.h"
#include <QResizeEvent>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QVBoxLayout>
#include <QToolButton>
#include <QLineEdit>
#include <QStack>
#include <QEvent>
#include <QFileDialog>


namespace Ui {
class FileView;
}

class FileView : public QWidget
{
    Q_OBJECT

public:
    explicit FileView(QWidget *parent = 0);
    ~FileView();

private:
    Ui::FileView *ui;
    QVBoxLayout *m_layout;

    QTableView *m_fileTable = NULL;
    FileTableModel *m_model = NULL;
    FileHtmlDelegate *m_delegate = NULL;


protected:
    bool eventFilter(QObject *obj, QEvent *event);


private Q_SLOTS:
    void onResizeTableView();

public slots:
    void onDoubleClicked(const QModelIndex & index);
    void onDataUpdated(QJsonObject fileData);
    void onSdCartListComing(QJsonArray arry);

    void resizeEvent(QResizeEvent* evt);
    void onContextMenuEvent(const QPoint &point);

    void onNewFile();
    void onRemoveFile();
    void onRenameFile();
    void onDownloadFile();
    void onUploadFile();

    void onCopyFile();
    void onCutFile();
    void onPasteFile();

    void onBack();
    void onNext();
    void onRefresh();

private:
    const int FILE_VIEW_WIDTH = 155;
    const int FILE_VIEW_HEIGHT = 135;

    QMenu *m_context_menu;
    QAction *m_rm_file_act;
    QAction *m_rename_file_act;
    QAction *m_download_file_act;
    QAction *m_upload_file_act;

    QString m_srcFilePath;
    //QString m_dstFilePath;
    int m_ifileOpMode;

    void createActions();
    void initToolBar();

    QLineEdit *m_folderPath;

    QString m_sCurrentPath;
    QString m_curRootPath;

    QStack <QString> m_history;
};





#endif // FILEMANAGERView_H
