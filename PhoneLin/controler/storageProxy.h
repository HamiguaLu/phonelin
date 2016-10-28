
#ifndef __STORAGE_PROXY_HELPER_HEADER__
#define __STORAGE_PROXY_HELPER_HEADER__

#include <QJsonObject>


#include "phonelin.h"


#include "phonelin.h"
#include "jsonHelper.h"
#include "deviceMonitor.h"


class StorageProxy
{
public:
    StorageProxy(DeviceMonitor *monitor,QString &devid);
    int getAudioList(ID_TYPE start_id);
    int getImageList(ID_TYPE start_id);
    int getVideoList(ID_TYPE start_id);

    int delFile(QString &fullPath);
    int renameFile(QString &preName,QString &newName);
    int newFile(QString &fileName,bool isDir);
	int copyFile(QString &srcName,QString &dstName);
	int cutFile(QString &srcName,QString &dstName);
	int downloadFile(QString &srcDevicePath,QString &dstLocalPath);
	int uploadFile(QString &srcLocalPath,QString &dstDevicePath);

    int getFileList(QString &path,ID_TYPE start);

	int getSDCardList();

private:
    int sendCmd(QJsonObject &cmd);
	DeviceMonitor *m_monitor = NULL;
    QString m_devid = "";
	
	
};



#endif



