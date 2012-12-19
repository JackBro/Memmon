#ifndef MEMMON_H
#define MEMMON_H

#include <QMainWindow>
#include <QMenu>
#include <QComboBox>
#include <QToolBar>
#include <QToolButton>

#include "mmdef.h"
#include "selectcolumndialog.h"
#include "xprocesstable/xprocesstable.h"
#include "querymanager.h"
#include "memorymonitordock.h"
#include "logoutputwindow.h"
#include "infoquerydock.h"

class Memmon : public QMainWindow
{
    Q_OBJECT
public:
    explicit Memmon(QWidget *parent = 0);
    

protected:
    void closeEvent(QCloseEvent *);

private:
    void createWidgets();
    void initVars();
    void initSettings();
    void saveSettings();
    void initMenus();
    void initToolbars();

    // that's where we put the late initialization variables
    void initLateInitVars();

    QAction* createAction(const QString& strText,const QIcon& icon = QIcon(), const QString& strTip = QString());
    QToolButton* createToolButton(const QString& strText,const QIcon& icon,const QString& strTip = QString());

    void showSelectColumnDialog();
    void showLogWindow();
    void showMemoryUtilityWindow();
    void showHelp();
    void showAboutThis();
    void showWMIQueryWindow();
    void showPopupChart();
    void configToolbar(QToolBar* toolbar);
    void populateTable();
    void showDock(QDockWidget* dock);
    void showStatus(const QString& strStatus);
private:
    XProcessTable* _processTable;


    QMenu* _fileMenu;
    QAction* _exitAct;

    QMenu* _configMenu;
    QAction* _selectColumnAct;

    QMenu* _windowMenu;
    QAction* _memUtilAct;
    QAction* _logDockAct;
    QAction* _wmiQueryAct;

    QMenu* _aboutMenu;
    QAction* _helpAct;
    QAction* _aboutThisAct;

    QToolBar* _toolBar;
    QLabel* _updateIntervalLabel;
    QComboBox* _updateIntervalCombo;
    QToolButton* _startButton;
    QToolButton* _stopButton;
    QToolButton* _clearButton;
    QToolButton* _showPopupButton;

    QStringList _columns;

    SelectColumnDialog* _selectColumnDialog;
    QueryManager* _queryManager;

    MemoryMonitorDock* _mmDock;
    LogOutputWindow* _logDock;
    InfoQueryDock* _wmiQueryDock;



private Q_SLOTS:
    void slot_toolbuttonHandler();
    void slot_actionHandler();
    void slot_setColumns(const QStringList& columns);
    void slot_updateIntervalChanged(int index);
    void slot_queryStopped();


};

#endif // MEMMON_H
