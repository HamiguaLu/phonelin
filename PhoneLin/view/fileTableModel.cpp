

#include <QJsonObject>
#include "fileTableModel.h"
#include <QDateTime>

#include "mimeTypes.h"


FileTableModel::FileTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}


int FileTableModel::setData(QJsonArray *data)
{
    beginResetModel();
    delete m_fileList;
    m_fileList = data;

    endResetModel();

    emit resizeView();

    return 0;
}

int FileTableModel::setItemCountPerRow(int c)
{
    beginResetModel();

    m_itemsPerRow = c;

    endResetModel();

    emit resizeView();
}



int FileTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if (m_fileList == NULL)
    {
        return 0;
    }

	int count = m_fileList->size();
    int rowCount = count/m_itemsPerRow + ((count % m_itemsPerRow > 0) ? 1:0);
    return rowCount;
}


int FileTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_itemsPerRow;
}



QString FileTableModel::cssTable()
{
    QString a;
    a = "<style type='text/css'>";
    a += "body {}";
    a +=" div { font-size:11px;}";
    a +=" .title {font-size:13px;font-weight:bold;}";
    a+="</style>";
    return a;
}


QString FileTableModel::formatItem(QJsonObject &file)
{
    QString info = "<body>";

    info.append(QString("<img src='%1'/>").arg(getIcon(file)));

    if (file.contains("name"))
    {
        info.append("<div class='title'>" + file.value("name").toString() + "</div>");
    }

    if (file.contains("lastModified"))
    {
        QDateTime timestamp;

        uint t = file.value("lastModified").toDouble()/1000;
        if (t != 0)
        {
            timestamp.setTime_t(t);
            QString modifiedTime = timestamp.toString(Qt::SystemLocaleShortDate);
            info.append(QString("<div align='center'>lastModified:%1</div>").arg(modifiedTime));
        }
    }

    info += "</body>";

    return info;
}

QString FileTableModel::getIcon(QJsonObject &file)
{
    if (!file.contains("isDir"))
    {
        return ":/file_type/file_icon/other-file.png";
    }

    bool isDir = file.value("isDir").toBool();
    if (isDir)
    {
        return ":/file_type/file_icon/folder.png";
    }

    if (!file.contains("name"))
    {
        return ":/file_type/file_icon/other-file.png";
    }

    QString name = file.value("name").toString();
    QString type = MimeTypes::getMimeTypes(name);

    if (type == "unknow")
    {
        return ":/file_type/file_icon/other-file.png";
    }

    if (type.contains("application"))
    {
        if (type.contains("vnd.android.package-archive"))
        {
            return ":/file_type/file_icon/apk.png";
        }

        if (type.contains("msword"))
        {
            return ":/file_type/file_icon/office.png";
        }

        if (type.contains("excel"))
        {
            return ":/file_type/file_icon/office.png";
        }

        if (type.contains("pdf"))
        {
            return ":/file_type/file_icon/pdf.png";
        }

        if (type.contains("zip"))
        {
            return ":/file_type/file_icon/zip.png";
        }

        if (type.contains("rar"))
        {
            return ":/file_type/file_icon/zip.png";
        }

        if (type.contains("x-tar"))
        {
            return ":/file_type/file_icon/zip.png";
        }

        return ":/file_type/file_icon/other-file.png";
    }

    if (type.contains("audio"))
    {
        return ":/file_type/file_icon/music.png";
    }

    if (type.contains("video"))
    {
        return ":/file_type/file_icon/video.png";
    }

    if (type.contains("text"))
    {
        return ":/file_type/file_icon/doc.png";
    }

    if (type.contains("image"))
    {
        return ":/file_type/file_icon/photo.png";
    }

    return ":/file_type/file_icon/other-file.png";
}




QVariant FileTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::TextAlignmentRole )
    {
        return QVariant(Qt::AlignCenter | Qt::AlignVCenter);
    }

    if (!index.isValid())
    {
        return QVariant();
    }

    int pos = index.row() * m_itemsPerRow  + index.column();
    if ( pos >= m_fileList->size() || index.row() < 0 || index.column() < 0)
    {
        return QVariant();
    }

    QJsonObject file = m_fileList->at(pos).toObject();
    if (role == Qt::UserRole)
    {
        return file;
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    QString html = cssTable() + formatItem(file);

    return QVariant(html);
}





#if 0


bool FileTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
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



Qt::ItemFlags FileTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractFileTableModel::flags(index) | Qt::ItemIsEditable;
}

#endif



