


#include <QJsonDocument>
#include <QJsonValue>
#include <QFileInfo>

#include "appProxy.h"



AppProxy::AppProxy(DeviceMonitor *monitor,QString &devid):m_monitor(monitor),m_devid(devid)
{

}



int AppProxy::getAppList(ID_TYPE start)
{
    QJsonObject cmd;

    cmd.insert("from",QString("%1").arg(start));

    return sendCmd(cmd,PL_REQ_GET_APP_LIST);

}
int AppProxy::getAppIcon(QString packageName)
{

    QJsonObject cmd;

    cmd.insert("packageName",QString("%1").arg(packageName));

    return sendCmd(cmd,PL_REQ_GET_APP_ICON);
}

int AppProxy::delApp(QString packageName)
{

    QJsonObject cmd;

    cmd.insert("packageName",QString("%1").arg(packageName));

    return sendCmd(cmd,PL_REQ_DEL_APP);
}

int AppProxy::installApp(QString &fullPath)
{
    QFileInfo fi;
    fi = QFileInfo(fullPath);
    QString dstFile = fi.fileName();

    return m_monitor->sendFile(m_devid,PL_REQ_INSTALL_APP,fullPath,dstFile);
}


int AppProxy::sendCmd(QJsonObject &cmd,int action)
{
    QJsonDocument doc(cmd);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
    return m_monitor->sendCmd(m_devid,action,&jsonData);
}






