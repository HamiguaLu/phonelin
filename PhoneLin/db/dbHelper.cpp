#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QJsonDocument>
#include <QJsonValue>
#include <QIODevice>
#include <QFile>


#include "dbHelper.h"


QSqlDatabase* DbHelper::m_db;
QString DbHelper::m_curDeviceID;


int DbHelper::init()
{
    open(PL_DB_NAME);
    initTable();

    return PL_RET_OK;
}

int DbHelper::deinit()
{
    QString connName = QSqlDatabase::database().connectionName();
    m_db->commit();
    m_db->close();
    delete m_db;

    QSqlDatabase::removeDatabase(connName);
    return PL_RET_OK;
}


int DbHelper::open(QString dbName)
{
    m_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    m_db->setDatabaseName(dbName);
    m_db->setUserName("root");
    m_db->setPassword("123456");

    if(!m_db->open())
    {
        DEBUG_INFO << m_db->lastError();
        qFatal("failed to open db") ;
    }

    return PL_RET_OK;
}



int DbHelper::initTable()
{
    /*{"body":"Reply TD to cancel","date":"1461003626274","_id":"214","address":"106908385805","read":"0","thread_id":"43"}*/
    QString createSmsTable = "CREATE TABLE if not exists SMS(_id INT NOT NULL,\
            body TEXT, \
            date TEXT, \
            address  TEXT,\
            read  TEXT,\
            thread_id TEXT,\
            device_id TEXT)";
            int ret = exec(createSmsTable);
    if (PL_RET_OK != ret)
    {
        return ret;
    }

    /*
     *
        "times_contacted":"0",
        "contact_last_updated_timestamp":"1460856888528",
        "last_time_contacted":"0",
        "_id":"3",
        "photo_uri":"content:\/\/com.android.contacts\/display_photo\/1",
        "display_name":"华为客服",
     */
    QString createContactTable = "CREATE TABLE if not exists CONTACT(_id INT NOT NULL,\
            times_contacted TEXT, \
            contact_last_updated_timestamp INT, \
            last_time_contacted  INT,\
            photo_uri  TEXT,\
            photo_thumb_uri TEXT,\
            display_name TEXT,\
            device_id TEXT)";
            ret = exec(createContactTable);
    if (PL_RET_OK != ret)
    {
        return ret;
    }

    QString createContactDataTable = "CREATE TABLE if not exists CONTACT_DATA(_id INT NOT NULL,\
            contact_id INT NOT NULL,mimetype  TEXT";

            for (int i = 1; i <= MAX_CONTACT_DATA_COUNT; ++i)
    {
        createContactDataTable += QString(",data%1 TEXT").arg(i);
    }

    createContactDataTable += ",device_id TEXT)";
    ret = exec(createContactDataTable);
    if (PL_RET_OK != ret)
    {
        return ret;
    }

    /*{"ring_times":"8",
    "formatted_number":"0755 86252486",
    "number":"075586252486",
    "matched_number":"075586252486",
    "date":"1460973239024",
    "type":"3",
    "geocoded_location":"广东深圳",
    "countryiso":"CN",
    "duration":"0",
    "new":"0",
    "numbertype":"0",
    "_id":"453",
    "subscription":"0",
    "presentation":"1",
    "is_private":"0",
    "is_read":"1",
    }*/

    QString createCallTable = "CREATE TABLE if not exists CALL_LOG(_id INT NOT NULL,\
            ring_times  TEXT, \
            formatted_number  TEXT ,\
            number  TEXT ,\
            matched_number  TEXT ,\
            date  TEXT ,\
            type  TEXT ,\
            geocoded_location  TEXT ,\
            countryiso  TEXT ,\
            duration  TEXT ,\
            numbertype  TEXT ,\
            is_private  TEXT ,\
            is_read  TEXT,\
            device_id TEXT)";
            ret = exec(createCallTable);
    if (PL_RET_OK != ret)
    {
        return ret;
    }


    QString createDeviceListTable = "CREATE TABLE if not exists DEVICE_LIST(device_name TEXT,\
            device_id  TEXT, \
            last_connected_time TIMESTAMP)";
            ret = exec(createDeviceListTable);
    if (PL_RET_OK != ret)
    {
        return ret;
    }

    return PL_RET_OK;
}


int DbHelper::exec(QString &sql)
{
    QSqlQuery q = m_db->exec(sql);
    if(!q.lastError().isValid())
    {
        while(q.next())
        {
            qDebug()<<q.value(8).toString();
        }
    }
    else
    {
        DEBUG_INFO <<"db failed to exec! , error: "<<q.lastError().text();
    }

    return PL_RET_OK;
}



int DbHelper::getDeviceList(QJsonArray &jsonAry)
{

    QString sql = QString("select device_name,DISTINCT device_id from DEVICE_LIST order by last_connected_time desc");

    int ret = query(sql,jsonAry);
    if (ret != PL_RET_OK || jsonAry.size() < 1)
    {
        return ret;
    }
    return PL_RET_OK;
}

int DbHelper::insertDevice(QString devName,QString devID)
{
    QSqlQuery query(*m_db);
    QString sql = "INSERT INTO DEVICE_LIST (device_name,device_id)";
    sql += " VALUES (?,?)";
    if (!query.prepare(sql))
    {
        DEBUG_INFO <<"db failed to exec! , error: "<<query.lastError().text();
        return PL_RET_FAIL;
    }

    query.bindValue(0, devName);
    query.bindValue(1, devID);

    //query.bindValue(1, last_connected_time);
    if (!query.exec() || query.lastError().isValid())
    {
        DEBUG_INFO <<"db failed to exec! , error: "<<query.lastError().text();
        return PL_RET_FAIL;
    }

    return PL_RET_OK;
}


int DbHelper::insertContact(QJsonObject &contact)
{
    QSqlQuery query(*m_db);
    QString sql = "INSERT INTO CONTACT (_id,times_contacted,contact_last_updated_timestamp, last_time_contacted, photo_uri,photo_thumb_uri,display_name,device_id)";
    sql += " VALUES (?, ?, ?, ?, ?, ?, ?,?)";
    if (!query.prepare(sql))
    {
        DEBUG_INFO <<"db failed to exec! , error: "<<query.lastError().text();
        return PL_RET_FAIL;
    }

    query.bindValue(0, contact.value("_id").toString());
    query.bindValue(1, contact.value("times_contacted").toString());
    query.bindValue(2, contact.value("contact_last_updated_timestamp").toString());
    query.bindValue(3, contact.value("last_time_contacted").toString());
    query.bindValue(4, contact.value("photo_uri").toString());
    query.bindValue(5, contact.value("photo_thumb_uri").toString());
    query.bindValue(6, contact.value("display_name").toString());
    query.bindValue(7, m_curDeviceID);
    if (!query.exec() || query.lastError().isValid())
    {
        DEBUG_INFO <<"db failed to exec! , error: "<<query.lastError().text();
        return PL_RET_FAIL;
    }

    return PL_RET_OK;
}




int DbHelper::insertContactData(QJsonObject &cData)
{
    QSqlQuery query(*m_db);
    QString sql = "INSERT INTO CONTACT_DATA (_id,contact_id,mimetype";

    for (int i = 1; i <= MAX_CONTACT_DATA_COUNT; ++i)
    {
        sql += QString(",data%1").arg(i);
    }

    sql += 	",device_id)";
    sql += " VALUES (?";
    for (int i = 1; i < MAX_CONTACT_DATA_COUNT + 3; ++i)
    {
        sql += ",?";
    }

    sql += ",?)";

    if (!query.prepare(sql))
    {
        DEBUG_INFO <<"db failed to exec! , error: "<<query.lastError().text();
        return PL_RET_FAIL;
    }

    query.bindValue(0, cData.value("_id").toString());
    query.bindValue(1, cData.value("contact_id").toString());
    query.bindValue(2, cData.value("mimetype").toString());

    for (int i = 1; i <= MAX_CONTACT_DATA_COUNT; ++i)
    {
        int idx = 2 + i;
        QString col = QString("data%1").arg(i);
        if (cData.contains(col))
        {
            query.bindValue(idx, cData.value(col).toString());
        }
        else
        {
            query.bindValue(idx, "");
        }
    }

    query.bindValue(3 + MAX_CONTACT_DATA_COUNT, m_curDeviceID);

    if (!query.exec() || query.lastError().isValid())
    {
        DEBUG_INFO <<"insertContactData:db failed to exec! , error: "<<query.lastError().text();
        return PL_RET_FAIL;
    }

    return PL_RET_OK;
}




int DbHelper::insertCall(QJsonObject &call)
{
    QSqlQuery query(*m_db);
    QString sql = "INSERT INTO CALL_LOG(_id, ring_times, formatted_number, \
            number,  matched_number,  date,   type,   geocoded_location, \
            countryiso,  duration, numbertype,   is_private, is_read,device_id)";
            sql += "VALUES (?, ?, ?, ?, ?, ?,?, ?, ?, ?, ?, ?,?,?)";
    if (!query.prepare(sql))
    {
        DEBUG_INFO <<"db failed to exec! , error: "<<query.lastError().text();
        return PL_RET_FAIL;
    }

    query.bindValue(0, call.value("_id").toString());
    query.bindValue(1, call.value("ring_times").toString());
    query.bindValue(2, call.value("formatted_number").toString());
    query.bindValue(3, call.value("number").toString());
    query.bindValue(4, call.value("matched_number").toString());
    query.bindValue(5, call.value("date").toString());
    query.bindValue(6, call.value("type").toString());
    query.bindValue(7, call.value("geocoded_location").toString());
    query.bindValue(8, call.value("countryiso").toString());
    query.bindValue(9, call.value("duration").toString());
    query.bindValue(10, call.value("numbertype").toString());
    query.bindValue(11, call.value("is_private").toString());
    query.bindValue(12, call.value("is_read").toString());
    query.bindValue(13, m_curDeviceID);
    if (!query.exec() || query.lastError().isValid())
    {
        DEBUG_INFO <<"db failed to exec! , error: "<<query.lastError().text();
        return PL_RET_FAIL;
    }

    return PL_RET_OK;
}


int DbHelper::insertSms(QJsonObject &sms)
{
    QSqlQuery query(*m_db);
    if (!query.prepare("INSERT INTO SMS (_id, body, date, address, read, thread_id,device_id )"
                       "VALUES (?, ?, ?, ?, ?, ?,?)"))
    {
        DEBUG_INFO <<"db failed to exec! , error: "<<query.lastError().text();
        return PL_RET_FAIL;
    }

    query.bindValue(0, sms.value("_id").toString());
    query.bindValue(1, sms.value("body").toString());
    query.bindValue(2, sms.value("date").toString());
    query.bindValue(3, sms.value("address").toString());
    query.bindValue(4, sms.value("read").toString());
    query.bindValue(5, sms.value("thread_id").toString());
    query.bindValue(6, m_curDeviceID);
    if (!query.exec() || query.lastError().isValid())
    {
        DEBUG_INFO <<"db failed to exec! , error: "<<query.lastError().text();
        return PL_RET_FAIL;
    }

    return PL_RET_OK;
}


int DbHelper::getMaxId(QString &table,ID_TYPE &max_id)
{
    QString sql = "select max(_id) from " + table;
    sql += " where device_id='" + m_curDeviceID + "'";
    QSqlQuery sql_query(*m_db);

    max_id = 0;

    if(!sql_query.prepare(sql) || !sql_query.exec(sql) || !sql_query.first())
    {
        DEBUG_INFO <<sql_query.lastError();
        return PL_RET_FAIL;
    }

    max_id = sql_query.value(0).toLongLong();

    return PL_RET_OK;
}

int DbHelper::getCallLogMaxId(ID_TYPE &max_id)
{
    QString table = "CALL_LOG";
    return getMaxId(table,max_id);
}


int DbHelper::getSmsMaxId(ID_TYPE &max_id)
{
    QString table = "SMS";
    return getMaxId(table,max_id);
}


int DbHelper::getContactMaxId(ID_TYPE &max_id)
{
    QString table = "CONTACT";
    return getMaxId(table,max_id);
}


int DbHelper::query(QString &sql,QJsonArray &jsonAry)
{
    QSqlQuery sql_query(*m_db);

    if(!sql_query.prepare(sql) || !sql_query.exec(sql) || !sql_query.first() )
    {
        //DEBUG_INFO << "Query failed or no data: " << sql << sql_query.lastError().text();
        return PL_RET_FAIL;
    }

    do
    {
        QJsonObject obj;
        QSqlRecord rec = sql_query.record();
        for (int i = 0; i < rec.count(); ++i)
        {
            obj.insert(rec.fieldName(i),sql_query.value(i).toString());
            //qDebug()<<QString("name:%1  value:%2").arg(rec.fieldName(i)).arg(sql_query.value(i).toString());
        }

        jsonAry.append(obj);
    }while(sql_query.next());

    sql_query.clear();

#if 0
    JsonHelper.dumpArray(jsonAry,"json3");
#endif

    return PL_RET_OK;
}


QString DbHelper::getNameByPhone(QString &phoneNo)
{
    QString sql = QString("select %1 from CONTACT_DATA where mimetype='%2' and contact_id=(select contact_id from CONTACT_DATA where mimetype='%3' and %4='%5' limit 0,1)")
            .arg(NAME_DISPLAY_NAME).arg(NAME_CONTENT_ITEM_TYPE)
            .arg(PHONE_CONTENT_ITEM_TYPE).arg(PHONE_NUMBER).arg(phoneNo);

    QJsonArray resultAry;
    int ret = query(sql,resultAry);
    if (PL_RET_OK != ret)
    {
        return "";
    }

    if (resultAry.size() != 1)
    {
        return "";
    }

    QJsonObject names = resultAry.at(0).toObject();
    if (names.contains(NAME_DISPLAY_NAME))
    {
        return names.value(NAME_DISPLAY_NAME).toString();
    }

    return "";
}


int DbHelper::loadSMSList(int from,int MAX_COUNT,QJsonArray &jsonAry)
{
    QString sql = QString("select * from SMS order by date desc limit %1, %2").arg(from).arg(MAX_COUNT);

    int ret = query(sql,jsonAry);
    if (ret != PL_RET_OK || jsonAry.size() < 1)
    {
        return ret;
    }

    for (int i = 0; i < jsonAry.size(); ++i)
    {
        QJsonObject c = jsonAry.at(i).toObject();

        QString number = c.value("address").toString();
        c.insert("name",getNameByPhone(number));
    }

    return PL_RET_OK;

}


int DbHelper::getContactData(QString mimeType, ID_TYPE contact_id,QJsonArray &jsonAry)
{
    QString sql = QString("select * from CONTACT_DATA where contact_id=%1 and mimeType='%2'").arg(contact_id).arg(mimeType);
    DEBUG_INFO <<"test";

    return query(sql,jsonAry);

}



int DbHelper::getContactProperty(QJsonObject &contact)
{
    QJsonArray phoneData,addrData;

    ID_TYPE _id = contact.value("_id").toString().toInt();
    if (getContactData(PHONE_CONTENT_ITEM_TYPE,_id,phoneData) == PL_RET_OK)
    {
        for (int j = 0; j < phoneData.size(); ++j)
        {
            QJsonObject d = phoneData.at(j).toObject();
#if 0
            JsonHelper.dumpObject(d,"json4");
#endif

            int type = d.value(BASE_TYPE_TYPE).toString().toInt();
            QString phoneNo = d.value(PHONE_NUMBER).toString();
            if (type == PHONE_TYPE_HOME)
            {
                contact.insert("homePhone",phoneNo);
            }
            else if (type == PHONE_TYPE_MOBILE)
            {
                contact.insert("mobilePhone",phoneNo);
            }
            else if (type == PHONE_TYPE_WORK)
            {
                contact.insert("workPhone",phoneNo);
            }
        }

    }


    if (getContactData(POSTAL_ADDR_CONTENT_ITEM_TYPE,_id,addrData) == PL_RET_OK)
    {
        for (int j = 0; j < addrData.size(); ++j)
        {
            QJsonObject d = addrData.at(j).toObject();
            int type = d.value(BASE_TYPE_TYPE).toString().toInt();

            QString address = "";
            if (d.contains(POSTAL_ADDR_STREET))
            {
                address += d.value(POSTAL_ADDR_STREET).toString();
                address += " ";
            }

            if (d.contains(POSTAL_ADDR_CITY))
            {
                address += d.value(POSTAL_ADDR_CITY).toString();
                address += " ";
            }

            if (d.contains(POSTAL_ADDR_REGION))
            {
                address += d.value(POSTAL_ADDR_REGION).toString();
                address += " ";
            }

            if (type == POSTAL_ADDR_TYPE_HOME)
            {
                contact.insert("homeAddr",address);
            }
            else if (type == POSTAL_ADDR_TYPE_WORK)
            {
                contact.insert("workAddr",address);
            }
            else if (type == POSTAL_ADDR_TYPE_OTHER)
            {
                contact.insert("otherAddr",address);
            }
        }
    }


    return PL_RET_OK;

}


int DbHelper::loadContactList(int from,int MAX_COUNT,QJsonArray &jsonAry)
{
    QString sql = QString("select * from CONTACT limit %1, %2").arg(from).arg(MAX_COUNT);
    int ret = query(sql,jsonAry);
    if (ret != PL_RET_OK || jsonAry.size() < 1)
    {
        return ret;
    }

    return PL_RET_OK;

}

int DbHelper::loadCallList(int from,int MAX_COUNT,QJsonArray &jsonAry)
{
    QString sql = QString("select * from CALL_LOG  order by date desc  limit %1, %2").arg(from).arg(MAX_COUNT);
    int ret = query(sql,jsonAry);
    if (ret != PL_RET_OK || jsonAry.size() < 1)
    {
        return ret;
    }

    for (int i = 0; i < jsonAry.size(); ++i)
    {
        QJsonObject c = jsonAry.at(i).toObject();
        //QJsonArray phoneData,addrData,nameData;

        QString number = c.value("number").toString();
        c.insert("name",getNameByPhone(number));
    }

    return PL_RET_OK;

}


int DbHelper::getSMSCount()
{
    return getRecordCount("SMS");
}

int DbHelper::getCallCount()
{
    return getRecordCount("CALL_LOG");
}


int DbHelper::getContactCount()
{
    return getRecordCount("CONTACT");
}



int DbHelper::getRecordCount(QString table)
{
    QString sql = "select count(_id) as c from " + table;
    QJsonArray result;
    int ret = query(sql,result);
    if (ret != PL_RET_OK || result.size() != 1)
    {
        return 0;
    }

    QJsonObject c = result.at(0).toObject();

    return c.value("c").toString().toInt();
}





