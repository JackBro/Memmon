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
#include "mmuiproxy.h"
#include "mmvarproxy.h"
#include "cpuusagefetcher.h"
#include "memoryusagefetcher.h"

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
    void initUsageFetcher();
    void initConnections();

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

    void setupStatusbar();
    void updateStatus(bool running);
    void exportContents();
private:
    XProcessTable* _processTable;

    SelectColumnDialog* _selectColumnDialog;
    QueryManager* _queryManager;

    MemoryMonitorDock* _mmDock;
    LogOutputWindow* _logDock;
    InfoQueryDock* _wmiQueryDock;

    MmUiProxy* _uiProxy;
    MmVarProxy _varProxy;

    CpuUsageFetcher* _cpuUsageFetcher;
    MemoryUsageFetcher* _memUsageFetcher;


private Q_SLOTS:
    void slot_toolbuttonHandler();
    void slot_actionHandler();
    void slot_setColumns(const QStringList& columns);
    void slot_updateIntervalChanged(int index);
    void slot_queryStopped();
    void slot_updateCpuUsage(int usage);
    void slot_updateMemUsage(int usage);
    void slot_showCpuUsageHistory();
    void slot_showMemUsageHistory();
    friend class MmUiProxy;

};

#endif // MEMMON_H
