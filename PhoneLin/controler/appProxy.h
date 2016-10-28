
#ifndef __APP_PROXY_HELPER_HEADER__
#define __APP_PROXY_HELPER_HEADER__

#include <QJsonObject>


#include "phonelin.h"


#include "phonelin.h"
#include "jsonHelper.h"
#include "deviceMonitor.h"


class AppProxy
{
public:
    AppProxy(DeviceMonitor *monitor,QString &devid);

    int getAppList(ID_TYPE start);
    int getAppIcon(QString packageName);
    int delApp(QString packageName);

    int installApp(QString &fullPath);

private:
    int sendCmd(QJsonObject &cmd,int action);
	DeviceMonitor *m_monitor = NULL;
    QString m_devid = "";
	
	
};



#endif



