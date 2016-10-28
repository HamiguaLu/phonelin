

#ifndef CONTACT_TABLE_MODEL_H
#define CONTACT_TABLE_MODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QPair>
#include <QJsonArray>
#include <QJsonObject>
#include "phonelin.h"
#include "../db/dbHelper.h"


class ContactTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    ContactTableModel(QObject *parent = 0);

    int setData(QJsonArray *data);
	int setItemCountPerRow(int c);

    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

	QJsonArray *m_ContactList = NULL;

	static QString cssTable();
	static QString formatContact(QJsonObject &contact);


public:
	int m_itemsPerRow = 5;


Q_SIGNALS:
    void resizeView();
};


#endif // TABLEMODEL_H
