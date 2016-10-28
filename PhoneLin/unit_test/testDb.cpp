
#include "db/dbHelper.h"
#include "testDb.h"


int testDb()
{
    ID_TYPE max_id = 0;


    QString sql = "select * from CALL_LOG";
    QJsonArray  jsonAry;
    DbHelper::query(sql,jsonAry);

    for(auto&& item: jsonAry)
    {
        QJsonObject jsonObj = item.toObject();

        QStringList l =  jsonObj.keys();
        foreach (QString key , l)
        {
            DEBUG_INFO << key << ":" << jsonObj.value(key).toString();
        }
    }

	DbHelper::getCallLogMaxId(max_id);

}
