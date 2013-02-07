#ifndef LLNOTIFYWIDGET_H
#define LLNOTIFYWIDGET_H

#include <QWidget>

class QShowEvent;
class QHideEvent;
class QAction;
class LLNotifyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LLNotifyWidget(QWidget *parent = 0);

public:
    void setAction(QAction* action);
    QAction* action() const;

protected:
    virtual void showEvent(QShowEvent* e);
    virtual void hideEvent(QHideEvent* e);
    virtual void keyPressEvent(QKeyEvent *);

private:
    QAction* _action;
};

#endif // LLNOTIFYWIDGET_H
