#include "monitorThread.h"

#include <QtNetwork>


MonitorThread::MonitorThread(QObject *parent)
{
    setParent(parent);

}

void MonitorThread::run()
{
    m_monitor = new DeviceMonitor(NULL);

    connect(m_monitor, SIGNAL(deviceDisconnected(QString)), parent(), SLOT(onDeviceDisconnected(QString)),Qt::QueuedConnection);
    connect(m_monitor, SIGNAL(deviceConnected(QString)), parent(), SLOT(onDeviceConnected(QString)),Qt::QueuedConnection);
    connect(m_monitor, SIGNAL(busyMessage(QString)), parent(), SLOT(onBusyMessage(QString)),Qt::QueuedConnection);
    connect(m_monitor, SIGNAL(errorMessage(QString,QString)), parent(), SLOT(onShowErrorInfo(QString,QString)),Qt::QueuedConnection);

    connect(this, SIGNAL(monitorCreated(DeviceMonitor *)), parent(), SLOT(onMonitorCreated(DeviceMonitor*)),Qt::QueuedConnection);
    emit monitorCreated(m_monitor);
    m_monitor->start();
    exec();
}
