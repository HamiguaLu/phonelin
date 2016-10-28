
#ifndef __JSON_OBJ_PARSER_HEADER__
#define __JSON_OBJ_PARSER_HEADER__

#include <QtCore/QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "phonelin.h"

class JsonHelper
{
public:
    JsonHelper();
    JsonHelper(QJsonObject obj);
    int parseObject(QByteArray *json_data);
    int parseArray(QByteArray *json_data);
    QJsonValue getValue(QString key);

    QJsonArray  m_jsonAry;
    int arraySize();

    QJsonObject m_jsonObj;

public:
	static void dumpObject(QJsonObject &o,QString fileName);
	static void dumpArray(QJsonArray &arry,QString fileName);
	static void dumpDoc(QJsonDocument doc,QString fileName);

};




#endif



