

#ifndef __FILE_HTML_DELEGATE__
#define __FILE_HTML_DELEGATE__


#include <QStyledItemDelegate>
#include "phonelin.h"

class FileHtmlDelegate : public QStyledItemDelegate
{
protected:
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};


#endif


