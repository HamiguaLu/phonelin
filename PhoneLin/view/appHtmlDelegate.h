

#ifndef __APP_HTML_DELEGATE__
#define __APP_HTML_DELEGATE__


#include <QStyledItemDelegate>
#include "phonelin.h"

class AppHtmlDelegate : public QStyledItemDelegate
{
protected:
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};


#endif


