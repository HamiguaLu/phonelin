


#include <QJsonDocument>
#include <QJsonValue>

#include "storageProxy.h"



StorageProxy::StorageProxy(DeviceMonitor *monitor,QString &devid):m_monitor(monitor),m_devid(devid)
{
	
}



int StorageProxy::sendCmd(QJsonObject &cmd)
{
    QJsonDocument doc(cmd);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
    return m_monitor->sendCmd(m_devid,PL_REQ_GET_MEDIA_LIST,&jsonData);
}




/*MediaStore.Audio.Media.EXTERNAL_CONTENT_URI*/
int StorageProxy::getAudioList(ID_TYPE start_id)
{
	QJsonObject cmd;

	cmd.insert("media_action",MEDIA_ACTION_GET_AUDIO_LIST);
    cmd.insert("uri",QString("content://sms/"));
    cmd.insert("selection",QString("_id>=?"));
    cmd.insert("argCount",1);

	QJsonObject arg;
    arg.insert("arg0",QString("%1").arg(start_id));
	cmd.insert("argument",arg);
    cmd.insert("sort_order",QString("_id desc"));

    return sendCmd(cmd);

}


/*MediaStore.Images.Media.EXTERNAL_CONTENT_URI*/
int StorageProxy::getImageList(ID_TYPE start_id)
{
    QJsonObject cmd;

    cmd.insert("media_action",MEDIA_ACTION_GET_IMAGE_LIST);
    cmd.insert("uri",QString("content://call/"));
    cmd.insert("selection",QString("_id>=?"));
    cmd.insert("argCount",1);

    QJsonObject arg;
    arg.insert("arg0",QString("%1").arg(start_id));
    cmd.insert("argument",arg);
    cmd.insert("sort_order",QString("_id desc"));

    return sendCmd(cmd);
}


/*MediaStore.Video.Media.EXTERNAL_CONTENT_URI*/
int StorageProxy::getVideoList(ID_TYPE start_id)
{
    QJsonObject cmd;

    cmd.insert("media_action",MEDIA_ACTION_GET_VIDEO_LIST);
    cmd.insert("uri",QString("content://com.android.contacts/contacts"));
    cmd.insert("selection",QString("_id>=?"));
    cmd.insert("argCount",1);

    QJsonObject arg;
    arg.insert("arg0",QString("%1").arg(start_id));
    cmd.insert("argument",arg);
    cmd.insert("sort_order",QString("_id desc"));

    return sendCmd(cmd);

}


 
int StorageProxy::delFile(QString &fileName)
{
	QJsonObject cmd;

    cmd.insert("path",fileName);
    cmd.insert("recusive",0);


	QJsonDocument doc(cmd);
	QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
	return m_monitor->sendCmd(m_devid,PL_REQ_DEL_FILE,&jsonData);
}


int StorageProxy::renameFile(QString &preName,QString &newName)
{
	QJsonObject cmd;

    cmd.insert("path",preName);
    cmd.insert("newName",newName);


	QJsonDocument doc(cmd);
	QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
	return m_monitor->sendCmd(m_devid,PL_REQ_RENAME_FILE,&jsonData);
}

int StorageProxy::newFile(QString &fileName,bool isDir)
{
	QJsonObject cmd;

    cmd.insert("path",fileName);
    cmd.insert("isDir",isDir);


	QJsonDocument doc(cmd);
	QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
	return m_monitor->sendCmd(m_devid,PL_REQ_NEW_FILE,&jsonData);
}

int StorageProxy::downloadFile(QString &srcDevicePath,QString &dstLocalPath)
{
	QJsonObject cmd;
    cmd.insert("path",srcDevicePath);
    cmd.insert("dstPath",dstLocalPath);

	QJsonDocument doc(cmd);
	QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
	return m_monitor->sendCmd(m_devid,PL_REQ_DOWNLOAD_FILE,&jsonData);
}



int StorageProxy::copyFile(QString &srcName,QString &dstName)
{
	QJsonObject cmd;
    cmd.insert("path",srcName);
    cmd.insert("dstPath",dstName);

	QJsonDocument doc(cmd);
	QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
	return m_monitor->sendCmd(m_devid,PL_REQ_COPY_FILE,&jsonData);

}

int StorageProxy::cutFile(QString &srcName,QString &dstName)
{
	QJsonObject cmd;
    cmd.insert("path",srcName);
    cmd.insert("dstPath",dstName);

	QJsonDocument doc(cmd);
	QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
	return m_monitor->sendCmd(m_devid,PL_REQ_CUT_FILE,&jsonData);

}



int StorageProxy::uploadFile(QString &srcLocalPath,QString &dstDevicePath)
{
    return m_monitor->sendFile(m_devid,PL_REQ_UPLOAD_FILE,srcLocalPath,dstDevicePath);
}






int StorageProxy::getFileList(QString &path,ID_TYPE start)
{
    QJsonObject cmd;

    cmd.insert("path",path);
    cmd.insert("from",QString("%1").arg(start));

	QJsonDocument doc(cmd);
	QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
	return m_monitor->sendCmd(m_devid,PL_REQ_GET_FILE_LIST,&jsonData);
}



int StorageProxy::getSDCardList()
{
    QJsonObject cmd;

    //cmd.insert("path","");

	QJsonDocument doc(cmd);
	QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
	return m_monitor->sendCmd(m_devid,PL_REQ_GET_SD_CARD_LIST,&jsonData);
}






