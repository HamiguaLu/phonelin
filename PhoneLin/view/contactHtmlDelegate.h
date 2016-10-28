

#ifndef __CONTACT_HTML_DELEGATE__
#define __CONTACT_HTML_DELEGATE__


#include <QStyledItemDelegate>
#include "phonelin.h"

class ContactHtmlDelegate : public QStyledItemDelegate
{
protected:
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};


#endif


