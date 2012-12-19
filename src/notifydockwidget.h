#ifndef NOTIFYDOCKWIDGET_H
#define NOTIFYDOCKWIDGET_H

#include <QDockWidget>
#include <QAction>

class NotifyDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit NotifyDockWidget(QWidget* parent = 0);
    explicit NotifyDockWidget(const QString& strTitle,QWidget* parent = 0);

public:
    void setAction(QAction* action);
    QAction* action() const;

protected:
    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);

private:
    QAction* _assocAct;
};

#endif // NOTIFYDOCKWIDGET_H
