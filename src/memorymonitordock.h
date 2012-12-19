#ifndef MEMORYMONITORDOCK_H
#define MEMORYMONITORDOCK_H

#include <QDockWidget>

#include "memorymonitorwindow.h"
#include "notifydockwidget.h"

class MemoryMonitorDock : public NotifyDockWidget
{
    Q_OBJECT
public:
    explicit MemoryMonitorDock(QWidget *parent = 0);
    

private:
    MemoryMonitorWindow* _mm;
    
};

#endif // MEMORYMONITORDOCK_H
