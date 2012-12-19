#ifndef INFOQUERYDOCK_H
#define INFOQUERYDOCK_H

#include "notifydockwidget.h"
#include "infoquerywindow.h"

class InfoQueryDock : public NotifyDockWidget
{
    Q_OBJECT
public:
    explicit InfoQueryDock(QWidget *parent = 0);
    

private:
    InfoQueryWindow* _infoQueryWindow;
};

#endif // INFOQUERYDOCK_H
