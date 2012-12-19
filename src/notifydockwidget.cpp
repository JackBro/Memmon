#include "notifydockwidget.h"

NotifyDockWidget::NotifyDockWidget(QWidget *parent) :
    QDockWidget(parent)
{
    _assocAct = 0;
}

NotifyDockWidget::NotifyDockWidget(const QString &strTitle, QWidget *parent):QDockWidget(strTitle,parent)
{
    _assocAct = 0;
}

void NotifyDockWidget::showEvent(QShowEvent *e)
{
    QDockWidget::showEvent(e);
    if(_assocAct){ _assocAct->setChecked(true);}
}
void NotifyDockWidget::hideEvent(QHideEvent *e)
{
    QDockWidget::hideEvent(e);
    if(_assocAct){ _assocAct->setChecked(false);}
}


void NotifyDockWidget::setAction(QAction *act)
{
    _assocAct = act;
}

QAction* NotifyDockWidget::action() const
{
    return _assocAct;
}
