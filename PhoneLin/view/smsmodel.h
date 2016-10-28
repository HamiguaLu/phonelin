

#ifndef SMS_TABLE_MODEL_H
#define SMS_TABLE_MODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QPair>
#include <QJsonArray>
#include <QJsonObject>
#include "phonelin.h"
#include "../db/dbHelper.h"



class SmsTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    SmsTableModel(QObject *parent = 0);

    int setData(QJsonArray *data);

    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

	QJsonArray *m_SmsList = NULL;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;


};


#endif // TABLEMODEL_H
