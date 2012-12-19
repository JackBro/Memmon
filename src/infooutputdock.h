#ifndef INFOOUTPUTDOCK_H
#define INFOOUTPUTDOCK_H

#include <QDockWidget>
#include <QToolBar>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QAction>

#include "notifydockwidget.h"

class InfoOutputDock : public NotifyDockWidget
{
    Q_OBJECT
public:
    explicit InfoOutputDock(const QString& strTitle,QWidget* parent = 0);
    explicit InfoOutputDock(QWidget *parent = 0);
    
public:
    void setText(const QString& str);
    QString text() const;

    void append(const QString& str);

    static QAction* createAction(const QString& strText,const QIcon& icon = QIcon(),const QString& strTip = QString(),QWidget* parent = 0);


private:
    void initWidgets();
    void initSettings();
    void initActions();
    void initConnections();


    QWidget* _mainWidget;
    QVBoxLayout* _widgetLayout;
    QToolBar* _toolbar;
    QTextEdit* _edit;

    QAction* _clearAct;
    QAction* _saveAct;

private Q_SLOTS:
    void clear();
    void save();
};

#endif // INFOOUTPUTDOCK_H
