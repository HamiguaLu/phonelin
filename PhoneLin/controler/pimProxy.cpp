

#include "pimProxy.h"
#include <QJsonDocument>
#include <QJsonValue>

PimProxy::PimProxy(DeviceMonitor *monitor,QString &devid):m_monitor(monitor),m_devid(devid)
{

}

int PimProxy::sendCmd(QJsonObject &cmd)
{
    QJsonDocument doc(cmd);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
    return m_monitor->sendCmd(m_devid,PL_REQ_GET_PIM_DATA,&jsonData);
}


int PimProxy::getSmsList(ID_TYPE start_id)
{
    QJsonObject cmd;

    cmd.insert("pim_action",PIM_ACTION_GET_SMS_LIST);
    cmd.insert("uri",QString("content://sms/"));
    cmd.insert("selection",QString("_id>=?"));
    cmd.insert("argCount",1);

    QJsonObject arg;
    arg.insert("arg0",QString("%1").arg(start_id));
    cmd.insert("argument",arg);
    cmd.insert("sort_order",QString("_id desc"));

    return sendCmd(cmd);

}


int PimProxy::delThread(ID_TYPE thread_id)
{
    return PL_RET_FAIL;
}


int PimProxy::delSms(ID_TYPE sms_id)
{
    return PL_RET_FAIL;
}

int PimProxy::sendSms(QString &receiver,QString &message)
{
    QJsonObject cmd;

    cmd.insert("pim_action",PIM_ACTION_SEND_SMS);
    cmd.insert("recipients",receiver);
    cmd.insert("content",message);

    QJsonDocument doc(cmd);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
    return m_monitor->sendCmd(m_devid,PL_REQ_SEND_SMS,&jsonData);

}


int PimProxy::getCallList(ID_TYPE start_id)
{
    QJsonObject cmd;

    cmd.insert("pim_action",PIM_ACTION_GET_CALL_LIST);
    cmd.insert("uri",QString("content://call_log/calls"));
    cmd.insert("selection",QString("_id>=?"));
    cmd.insert("argCount",1);

    QJsonObject arg;
    arg.insert("arg0",QString("%1").arg(start_id));
    cmd.insert("argument",arg);
    cmd.insert("sort_order",QString("_id desc"));

    return sendCmd(cmd);
}


int PimProxy::getContactList(ID_TYPE start_id)
{
    QJsonObject cmd;

    cmd.insert("pim_action",PIM_ACTION_GET_CONTACT_LIST);
    cmd.insert("uri",QString("content://com.android.contacts/contacts"));
    cmd.insert("selection",QString("_id>=?"));
    cmd.insert("argCount",1);

    QJsonObject arg;
    arg.insert("arg0",QString("%1").arg(start_id));
    cmd.insert("argument",arg);
    cmd.insert("sort_order",QString("_id desc"));

    return sendCmd(cmd);

}


int PimProxy::getSimContactList(ID_TYPE start_id)
{
    QJsonObject cmd;

    cmd.insert("pim_action",PIM_ACTION_GET_SIM_CONTACT_LIST);
    cmd.insert("uri",QString("content://icc/adn"));
    cmd.insert("selection",QString("_id>=?"));
    cmd.insert("argCount",1);

    QJsonObject arg;
    arg.insert("arg0",QString("%1").arg(start_id));
    cmd.insert("argument",arg);
    cmd.insert("sort_order",QString("_id desc"));

    return sendCmd(cmd);

}


int PimProxy::getContactDataList(ID_TYPE contact_id)
{
    QJsonObject cmd;

    cmd.insert("pim_action",PIM_ACTION_GET_CONTACT_DATA_LIST);
    cmd.insert("uri",QString("content://com.android.contacts/data"));//ContactsContract.Data.CONTENT_URI
    cmd.insert("selection",QString("contact_id>=?"));//ContactsContract.CommonDataKinds.StructuredName.CONTACT_ID
    cmd.insert("argCount",1);

    QJsonObject arg;
    arg.insert("arg0",QString("%1").arg(contact_id));
    cmd.insert("argument",arg);
    cmd.insert("sort_order",QString("_id desc"));

    return sendCmd(cmd);

}


int PimProxy::getContactEmailList(ID_TYPE contact_id)
{
    QJsonObject cmd;

    cmd.insert("pim_action",PIM_ACTION_GET_CONTACT_EMAIL_LIST);
    cmd.insert("uri",QString("content://com.android.contacts/data/emails"));//ContactsContract.CommonDataKinds.Email.CONTENT_URI
    cmd.insert("selection",QString("contact_id>=?"));//ContactsContract.CommonDataKinds.Phone.CONTACT_ID
    cmd.insert("argCount",1);

    QJsonObject arg;
    arg.insert("arg0",QString("%1").arg(contact_id));
    cmd.insert("argument",arg);
    cmd.insert("sort_order",QString("_id desc"));

    return sendCmd(cmd);

}


int PimProxy::getContactPhoneList(ID_TYPE contact_id)
{
    QJsonObject cmd;

    cmd.insert("pim_action",PIM_ACTION_GET_CONTACT_PHONE_LIST);
    cmd.insert("uri",QString("content://com.android.contacts/data/phones"));//ContactsContract.CommonDataKinds.Phone.CONTENT_URI
    cmd.insert("selection",QString("contact_id>=?"));//ContactsContract.CommonDataKinds.Phone.CONTACT_ID
    cmd.insert("argCount",1);

    QJsonObject arg;
    arg.insert("arg0",QString("%1").arg(contact_id));
    cmd.insert("argument",arg);
    cmd.insert("sort_order",QString("_id desc"));

    return sendCmd(cmd);

}


//ContactsContract.CommonDataKinds.StructuredPostal.CONTENT_URI,
//ContactsContract.CommonDataKinds.Phone.CONTACT_ID = contact_id
int PimProxy::getContactAddress(ID_TYPE contact_id)
{
    QJsonObject cmd;

    cmd.insert("pim_action",PIM_ACTION_GET_CONTACT_ADDR_LIST);
    cmd.insert("uri",QString("content://com.android.contacts/data/postals"));
    cmd.insert("selection",QString("contact_id>=?"));//ContactsContract.CommonDataKinds.Phone.CONTACT_ID
    cmd.insert("argCount",1);

    QJsonObject arg;
    arg.insert("arg0",QString("%1").arg(contact_id));
    cmd.insert("argument",arg);
    cmd.insert("sort_order",QString("_id desc"));

    return sendCmd(cmd);

}


int PimProxy::getContactPhoto(ID_TYPE contact_id)
{
    QJsonObject cmd;

    cmd.insert("contact_id",QString("%1").arg(contact_id));

    QJsonDocument doc(cmd);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
    return m_monitor->sendCmd(m_devid,PL_REQ_GET_PIM_PHOTO,&jsonData);

}


