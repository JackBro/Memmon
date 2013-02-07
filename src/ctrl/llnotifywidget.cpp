#include "llnotifywidget.h"

#include <QAction>
#include <QShowEvent>
#include <QHideEvent>

LLNotifyWidget::LLNotifyWidget(QWidget *parent) :
    QWidget(parent),_action(NULL)
{
}

/*!
 *
 * reimpl
 *
 */
void LLNotifyWidget::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);
    if(_action) { _action->setChecked(true); }
}

void LLNotifyWidget::hideEvent(QHideEvent *e)
{
    QWidget::hideEvent(e);
    if(_action) { _action->setChecked(false); }
}

void LLNotifyWidget::keyPressEvent(QKeyEvent *e)
{
    if(e->modifiers() & Qt::ControlModifier && e->key() == Qt::Key_W)
    {
        hide();
    }

    QWidget::keyPressEvent(e);
}

/*!
 *
 * public functions
 *
 */
void LLNotifyWidget::setAction(QAction *action)
{
    _action = action;
}

QAction* LLNotifyWidget::action() const
{
    return _action;
}
