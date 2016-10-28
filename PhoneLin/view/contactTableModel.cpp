

#include <QJsonObject>
#include "contactTableModel.h"
#include <QDir>


ContactTableModel::ContactTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}


int ContactTableModel::setData(QJsonArray *data)
{
    beginResetModel();
    delete m_ContactList;
    m_ContactList = data;
    endResetModel();

    emit resizeView();

    return 0;
}

int ContactTableModel::setItemCountPerRow(int c)
{
    beginResetModel();

    m_itemsPerRow = c;

    endResetModel();

    emit resizeView();
}


int ContactTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if (m_ContactList == NULL)
    {
        return 0;
    }

	int count = m_ContactList->size();
    int rowCount = count/m_itemsPerRow + ((count % m_itemsPerRow > 0) ? 1:0);
    return rowCount;
}


int ContactTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_itemsPerRow;
}



QString ContactTableModel::cssTable()
{
    QString a;
    a = "<style type=\"text/css\">";
    a += "body {background-color:white;margin-top:5px;margin-bottom:5px;margin-left:5px;margin-right:5px}";
    a +=" div {  font-size:11px;}";
    a +=" .title {  font-size:13px;font-weight:bold;}";
    a+="</style>";
    return a;
}


QString ContactTableModel::formatContact(QJsonObject &contact)
{
    QString info = "<body>";

    //info.append("<img src=':/icon/icon/outgoing_call.png'/>");
    if (contact.contains("_id"))
    {
	    QString name = QString("%1%2.png").arg(QDir::tempPath()).arg(contact.value("_id").toString());
        info.append("<img src='" + name + "/>");
    }	

    if (contact.contains("display_name"))
    {
        info.append("<div class='title' align=\"center\">" + contact.value("display_name").toString() + "</div>");
    }

    DbHelper::getContactProperty(contact);

    if (contact.contains("homePhone"))
    {
        info.append("<div>Home:" + contact.value("homePhone").toString() + "</div>");
    }

    if (contact.contains("mobilePhone"))
    {
        info.append("<div>Mobile:" + contact.value("mobilePhone").toString() + "</div>");
    }

    if (contact.contains("workPhone"))
    {
        info.append("<div>Work:" + contact.value("workPhone").toString() + "</div>");
    }

    info += "</body>";

    return info;
}



QVariant ContactTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    int pos = index.row() * m_itemsPerRow  + index.column();
    if ( pos >= m_ContactList->size() || index.row() < 0 || index.column() < 0)
    {
        return QVariant();
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    QJsonObject contact = m_ContactList->at(pos).toObject();

    QString html = cssTable() + formatContact(contact);

    return QVariant(html);
}





#if 0


bool ContactTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();

        QPair<QString, QString> p = listOfPairs.value(row);

        if (index.column() == 0)
            p.first = value.toString();
        else if (index.column() == 1)
            p.second = value.toString();
        else
            return false;

        listOfPairs.replace(row, p);
        emit(dataChanged(index, index));

        return true;
    }

    return false;
}



Qt::ItemFlags ContactTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractContactTableModel::flags(index) | Qt::ItemIsEditable;
}

#endif



