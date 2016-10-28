
#ifndef __PIM_HELPER_HEADER__
#define __PIM_HELPER_HEADER__

#include <QJsonObject>


#include "phonelin.h"


#include "phonelin.h"
#include "jsonHelper.h"
#include "deviceMonitor.h"


class PimProxy
{
public:
    PimProxy(DeviceMonitor *monitor,QString &devid);
    int getSmsList(ID_TYPE start_id);
    int getContactList(ID_TYPE start_id);

	int delThread(ID_TYPE thread_id);

	
	int delSms(ID_TYPE sms_id);
    int sendSms(QString &receiver,QString &message);
	
    int getCallList(ID_TYPE start_call_id);

    int getSimContactList(ID_TYPE start_id);
    int getContactDataList(ID_TYPE contact_id);
    int getContactEmailList(ID_TYPE contact_id);
    int getContactPhoneList(ID_TYPE contact_id);
    int getContactAddress(ID_TYPE contact_id);

    int getContactPhoto(ID_TYPE contact_id);

private:
    int sendCmd(QJsonObject &cmd);
	DeviceMonitor *m_monitor = NULL;
    QString m_devid = "";
	
	
};



#endif



