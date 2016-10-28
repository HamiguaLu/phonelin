
#include <QIODevice>
#include <QFile>

#include "jsonHelper.h"



JsonHelper::JsonHelper()
{
}

JsonHelper::JsonHelper(QJsonObject obj)
{
    m_jsonObj = obj;
}


int JsonHelper::parseArray(QByteArray *json_data)
{
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(*json_data,&json_error);
    if(json_error.error != QJsonParseError::NoError)
    {
        DEBUG_INFO << "Parse Json data failed:" << json_error.error;
        return PL_RET_FAIL;
    }

    if(!parse_doucment.isArray())
    {
        DEBUG_INFO << "not a array";
        return PL_RET_FAIL;
    }

    m_jsonAry = parse_doucment.array();
    return PL_RET_OK;
}

int JsonHelper::arraySize()
{
    return m_jsonAry.size();
}


int JsonHelper::parseObject(QByteArray *json_data)
{
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(*json_data,&json_error);
    if(json_error.error != QJsonParseError::NoError)
    {
        DEBUG_INFO << "Parse Json data failed:" << json_error.error;
        return PL_RET_FAIL;
    }

    if(!parse_doucment.isObject())
    {
        DEBUG_INFO << "Parse Json data failed: not object";
        return PL_RET_FAIL;
    }

#if 0
	dumpDoc(parse_doucment,"json1");
#endif

    m_jsonObj = parse_doucment.object();
    return PL_RET_OK;
}

QJsonValue JsonHelper::getValue(QString key)
{
    if (m_jsonObj.contains(key))
    {
        return m_jsonObj.value(key);
    }

    return QJsonValue::Null;
}





void JsonHelper::dumpObject(QJsonObject &o,QString fileName)
{
	fileName += ".json";

    QJsonDocument jsonDoc = QJsonDocument();
    jsonDoc.setObject(o);
	QFile file(fileName);
    file.open(QIODevice::Append);
    file.write(jsonDoc.toJson(QJsonDocument::Indented));
    file.close();
}


void JsonHelper::dumpArray(QJsonArray &arry,QString fileName)
{
	fileName += ".json";

    QJsonDocument jsonDoc = QJsonDocument();
    jsonDoc.setArray(arry);
	QFile file(fileName);
    file.open(QIODevice::Append);
    file.write(jsonDoc.toJson(QJsonDocument::Indented));
    file.close();
}

void JsonHelper::dumpDoc(QJsonDocument doc,QString fileName)
{
	fileName += ".json";

	QFile file(fileName);
    file.open(QIODevice::Append);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
}



