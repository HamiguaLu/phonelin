

#include <QJsonObject>
#include "smsmodel.h"
#include <QDateTime>

SmsTableModel::SmsTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}


int SmsTableModel::setData(QJsonArray *data)
{
	beginResetModel();
	delete m_SmsList;
    m_SmsList = data;
    endResetModel();

}


int SmsTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
	if (m_SmsList == NULL)
	{
		return 0;
	}
	
	return m_SmsList->size();

}


int SmsTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 6;
}



QVariant SmsTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    QJsonObject sms = m_SmsList->at(index.row()).toObject();
    switch (index.column())
    {
    case 0:
        return sms.value("_id").toString();
    case 1:
        return sms.value("name").toString();
    case 2:
        return sms.value("address").toString();
    case 3:
    {
        QDateTime timestamp;
        uint t = sms.value("date").toString().toLongLong()/1000;

        timestamp.setTime_t(t);
        return timestamp.toString(Qt::SystemLocaleShortDate);

    }
    case 4:
        return sms.value("body").toString();
    case 5:
        return sms.value("read").toString();
    }

    return QVariant("");
}


QVariant SmsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return "ID";
        case 1:
            return "Name";
        case 2:
            return "Phone";
        case 3:
            return "Date";
        case 4:
            return "Body";
        case 5:
            return "Read";
            default:
                return QVariant();
        }
    }
    return QVariant();
}


