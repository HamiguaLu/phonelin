

#ifndef APP_TABLE_MODEL_H
#define APP_TABLE_MODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QPair>
#include <QJsonArray>
#include <QJsonObject>
#include "phonelin.h"
#include "../db/dbHelper.h"


class AppTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    AppTableModel(QObject *parent = 0);

    int setIcon(QString &packageName,QString &iconPath);
    int setData(QJsonArray *data);
	int setItemCountPerRow(int c);

    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    QJsonArray *m_appList = NULL;

	static QString cssTable();
	static QString formatAppItem(QJsonObject &app);


public:
	int m_itemsPerRow = 5;


Q_SIGNALS:
    void resizeView();
};


#endif // TABLEMODEL_H
