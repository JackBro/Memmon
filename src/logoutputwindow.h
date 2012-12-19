#ifndef LOGOUTPUTWINDOW_H
#define LOGOUTPUTWINDOW_H

#include <QDockWidget>
#include <QTextEdit>

#include "notifydockwidget.h"

class LogOutputWindow : public NotifyDockWidget
{
    Q_OBJECT
public:
    explicit LogOutputWindow(QWidget *parent = 0);
    
public Q_SLOTS:

    void setText(const QString& strLog);
    void appendText(const QString& strLog);
    void clear();

private:
    void initLayout();

private:
    QTextEdit* _logEdit;
};

#endif // LOGOUTPUTWINDOW_H
