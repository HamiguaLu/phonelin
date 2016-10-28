

#include <QJsonObject>
#include "appTableModel.h"

#include <QDir>

AppTableModel::AppTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}


int AppTableModel::setData(QJsonArray *data)
{
    beginResetModel();
    delete m_appList;
    m_appList = data;
    endResetModel();

    emit resizeView();

    return 0;
}


int AppTableModel::setIcon(QString &packageName,QString &iconPath)
{
	if (m_appList == NULL)
	{
		return 0;
	}

    beginResetModel();

	for (int i = 0; i < m_appList->size(); ++i)
	{
		QJsonObject app = m_appList->at(i).toObject();
		if (!app.contains("packageName"))
		{
			continue;
		}

		if (app.value("packageName").toString() == packageName)
		{
			app.insert("icon",iconPath);
		}
	}
   
    endResetModel();

    emit resizeView();

    return 0;
}



int AppTableModel::setItemCountPerRow(int c)
{
    beginResetModel();

    m_itemsPerRow = c;

    endResetModel();

    emit resizeView();
}

int AppTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (m_appList == NULL)
    {
        return 0;
    }

    int count = m_appList->size();
    int rowCount = count/m_itemsPerRow + ((count % m_itemsPerRow > 0) ? 1:0);
    return rowCount;
}


int AppTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_itemsPerRow;
}



QString AppTableModel::cssTable()
{
    QString a;
    a = "<style type='text/css'>";
    a += "body {}";
    a +=" div { font-size:11px;}";
    a +=" .title {font-size:13px;font-weight:bold;}";
    a+="</style>";
    return a;
}


QString AppTableModel::formatAppItem(QJsonObject &app)
{
    QString info = "<body>";

    //if (app.contains("icon"))
	{
        QString name = QString("%1%2%3.png").arg(QDir::tempPath()).arg("//").arg(app.value("packageName").toString());
        QString imgElement = QString("<img width=42 height=42 src='%1'/>").arg(name);
	    info.append(imgElement);	
	}

    if (app.contains("appname"))
    {
        info.append("<div class='title' align=\"center\">" + app.value("appname").toString() + "</div>");
    }

    if (app.contains("versionName"))
    {
        info.append("<div>version:" + app.value("versionName").toString() + "</div>");
    }

    info += "</body>";

    return info;
}



QVariant AppTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    int pos = index.row() * m_itemsPerRow  + index.column();
    if ( pos >= m_appList->size() || index.row() < 0 || index.column() < 0)
    {
        return QVariant();
    }

    QJsonObject contact = m_appList->at(pos).toObject();

    if (role == Qt::UserRole)
    {
        return contact;
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    QString html = cssTable() + formatAppItem(contact);

    return QVariant(html);
}





#if 0


bool AppTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
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



Qt::ItemFlags AppTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractAppTableModel::flags(index) | Qt::ItemIsEditable;
}

#endif



