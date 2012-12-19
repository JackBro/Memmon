#include "memorymonitordock.h"
#include "mmdef.h"

MemoryMonitorDock::MemoryMonitorDock(QWidget *parent) :
    NotifyDockWidget(parent)
{
    _mm = new MemoryMonitorWindow(this);
    setWidget(_mm);
    setWindowTitle(MM::Text::Title_MMW);
}
