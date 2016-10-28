

#include <QJsonObject>
#include "calllogModel.h"
#include <QDateTime>
#include <QIcon>
#include <QPixmap>

CallLogModel::CallLogModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}


int CallLogModel::setData(QJsonArray *data)
{
    beginResetModel();
    delete m_CallList;
    m_CallList = data;
    endResetModel();

    return 0;
}


int CallLogModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if (m_CallList == NULL)
    {
        return 0;
    }

    return m_CallList->size();
}


int CallLogModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 6;
}


void formatTimeString(uint iTimeInSecs,QString &sTime)
{
    if (0 == iTimeInSecs)
    {
         sTime = "0 second";
         return;
    }

    uint dwHour = iTimeInSecs / 3600;
    uint dwMin  = (iTimeInSecs - dwHour * 3600)/60;
    int dwSec  = int(iTimeInSecs - dwHour * 3600 - dwMin * 60);

    sTime = "";
    if (dwHour > 0)
    {
        if (dwHour > 1)
        {
            sTime += QString("%1 hours ").arg(dwHour);
        }
        else
        {
            sTime += QString("%1 hours ").arg(dwHour);
        }
    }

    if (dwMin > 0)
    {
        if (dwMin > 1)
        {
            sTime += QString("%1 minutes ").arg(dwMin);
        }
        else
        {
            sTime += QString("%1 minute ").arg(dwMin);
        }
    }

    if (dwSec > 0)
    {
        if (dwSec > 1)
        {
            sTime += QString("%1 seconds ").arg(dwSec);
        }
        else
        {
            sTime += QString("%1 second ").arg(dwSec);
        }
    }

    return;
}


QVariant CallLogModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    QJsonObject call = m_CallList->at(index.row()).toObject();
    if (index.column() == 1 && role == Qt::DecorationRole)
    {
        int type = call.value("type").toString().toInt();
        switch (type)
        {
        case CALL_TYPE_INCOMING_CALL:
        {
            return QPixmap(":/icon/icon/incoming_call.png");
        }
        case CALL_TYPE_OUTGOING_CALL:
        {
            return QPixmap(":/icon/icon/outgoing_call.png");
        }
        case CALL_TYPE_MISSED_CALL:
        {
            return QPixmap(":/icon/icon/missed_call.png");
        }
        case CALL_TYPE_VOICE_MAIL:
        {
            return QPixmap(":/icon/icon/incoming_call.png");
        }
        case CALL_TYPE_REJECTED:
        {
            return QPixmap(":/icon/icon/rejected_call.png");
        }
        case CALL_TYPE_REFUSED:
        {
            return QPixmap(":/icon/icon/rejected_call.png");
        }
        }

    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    switch (index.column())
    {
    case 0:
        return call.value("_id").toString();
    case 1:
    {
        int type = call.value("type").toString().toInt();
        switch (type)
        {
        case CALL_TYPE_INCOMING_CALL:
        {
            return "Incoming";
        }
        case CALL_TYPE_OUTGOING_CALL:
        {
            return "Outgoing";
        }
        case CALL_TYPE_MISSED_CALL:
        {
            return "Missed";
        }
        case CALL_TYPE_VOICE_MAIL:
        {
            return "Voice";
        }
        case CALL_TYPE_REJECTED:
        {
            return "Rejected";
        }
        case CALL_TYPE_REFUSED:
        {
            return "Refused";
        }
        }

    }
    case 2:
        return call.value("name").toString();
    case 3:
        return call.value("number").toString();
    case 4:
    {
        QDateTime timestamp;
        timestamp.setTime_t(call.value("date").toString().toLongLong()/1000);
        return timestamp.toString(Qt::SystemLocaleShortDate);
    }
    case 5:
        uint duration = call.value("duration").toString().toUInt();
        QString sTime = "";
        formatTimeString(duration,sTime);
        return sTime;

    }

    return QVariant("");
}


QVariant CallLogModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return "ID";
        case 1:
            return "Type";
        case 2:
            return "Name";
        case 3:
            return "Phone";
        case 4:
            return "Date";
        case 5:
            return "Duration";
        default:
            return QVariant();
        }
    }
    return QVariant();
}





