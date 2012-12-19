#include "infoquerydock.h"
#include "mmdef.h"

InfoQueryDock::InfoQueryDock(QWidget *parent) :
    NotifyDockWidget(parent)
{
    _infoQueryWindow = new InfoQueryWindow(this);
    _infoQueryWindow->setWindowFlags(Qt::SubWindow);
    setWidget(_infoQueryWindow);

    setWindowTitle(MM::Text::Title_WMIQueryWindow);
}
