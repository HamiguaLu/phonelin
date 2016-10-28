
#ifndef __MIME_TYPES__
#define __MIME_TYPES__

#include <QString>
#include <QMap>

class MimeTypes
{
public:
	static void init();
    static QMap<QString,QString> m_map;
	
	static QString getMimeTypes(QString sFileName);
};

#endif
