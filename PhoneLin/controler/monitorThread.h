#ifndef MONITOR_THREAD_H
#define MONITOR_THREAD_H

#include <QThread>
#include "deviceMonitor.h"

class MonitorThread : public QThread
{
    Q_OBJECT

public:
    MonitorThread(QObject *parent);

    DeviceMonitor *m_monitor = NULL;

    void run() Q_DECL_OVERRIDE;
Q_SIGNALS:
    void monitorCreated(DeviceMonitor *m);

};

#endif
