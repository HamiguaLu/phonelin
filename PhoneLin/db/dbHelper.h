
#ifndef __PL_DB_HELPER_
#define __PL_DB_HELPER_


#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QSqlDatabase>
#include <QJsonObject>
#include <QJsonArray>


#include "phonelin.h"
#include "android_const.h"


class DbHelper
{
public:

	static int init();
	static int deinit();

	static int getDeviceList(QJsonArray &jsonAry);
	static int insertDevice(QString devName,QString devID);

    static int insertContact(QJsonObject &contact);
    static int insertContactData(QJsonObject &cData);
    static int getContactData(QString mimeType, ID_TYPE contact_id,QJsonArray &jsonAry);

    static int insertCall(QJsonObject &call);
    static int insertSms(QJsonObject &sms);
    static int query(QString &sql,QJsonArray &jsonAry);

    static int getCallLogMaxId(ID_TYPE &max_id);
    static int getSmsMaxId(ID_TYPE &max_id);
    static int getContactMaxId(ID_TYPE &max_id);


	static QString getNameByPhone(QString &phoneNo);
	static int loadSMSList(int from,int MAX_COUNT,QJsonArray &jsonAry);
	static int loadContactList(int from,int MAX_COUNT,QJsonArray &jsonAry);
	static int loadCallList(int from,int MAX_COUNT,QJsonArray &jsonAry);

	static int getSMSCount();
	static int getCallCount();
	static int getContactCount();
	static int getContactProperty(QJsonObject &contact);
    static int open(QString dbName);
    static int exec(QString &sql);
    static int initTable();

	static QString m_curDeviceID;

private:
    static QSqlDatabase *m_db;
    static int getMaxId(QString &table,ID_TYPE &max_id);
	static int getRecordCount(QString table);


#if 0
    QString update_sql = "update student set name = :name where id = :id";
    QString delete_sql = "delete from student where id = ?";
    QString clear_sql = "delete from student";
#endif
};


#endif

