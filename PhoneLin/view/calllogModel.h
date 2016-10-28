

#ifndef CALL_TABLE_MODEL_H
#define CALL_TABLE_MODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QPair>
#include <QJsonArray>
#include <QJsonObject>
#include "phonelin.h"
#include "../db/dbHelper.h"

class CallLogModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    CallLogModel(QObject *parent = 0);

    int setData(QJsonArray *data);

    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

	QJsonArray *m_CallList = NULL;

#if 0
	QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;
	Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) Q_DECL_OVERRIDE;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) Q_DECL_OVERRIDE;
    QList<QPair<QString, QString> > getList();
#endif

    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;

};


#endif // TABLEMODEL_H
