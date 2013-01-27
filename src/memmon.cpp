#include "memmon.h"
#include "mmdef.h"

#include <QSettings>
#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>

#undef USE_WIDGET
#define USE_WIDGET(WIDGET_NAME,WIDGET_ENUM) _uiProxy->get ## WIDGET_NAME(MmUiProxy :: WIDGET_NAME ## _ ## WIDGET_ENUM)

Memmon::Memmon(QWidget *parent) :
    QMainWindow(parent)
{
    initVars();
    initLateInitVars();
    createWidgets();
    initSettings();
    initMenus();
    initToolbars();
//    populateTable();
    setupStatusbar();
}

void Memmon::createWidgets()
{
    _processTable = new XProcessTable(this);
    setCentralWidget(_processTable);
    _queryManager->setTable(_processTable);
//    _queryManager->start();
}

void Memmon::initVars()
{
    _uiProxy = new MmUiProxy(this);
    _queryManager = new QueryManager(this);
    connect(_queryManager,SIGNAL(sig_queryStopped()),this,SLOT(slot_queryStopped()));
}

void Memmon::initSettings()
{
    setWindowTitle(tr("Memory Monitor"));
    showMaximized();
}

void Memmon::saveSettings()
{
    QByteArray states = saveState();

}

void Memmon::initMenus()
{

    /// add file menu
    USE_WIDGET(Menu,File)->addAction(USE_WIDGET(Action,Export));
    USE_WIDGET(Menu,File)->addSeparator();
    USE_WIDGET(Menu,File)->addAction(USE_WIDGET(Action,Exit));
    menuBar()->addMenu(USE_WIDGET(Menu,File));

    /// add config menu
    USE_WIDGET(Menu,Config)->addAction(USE_WIDGET(Action,SelectColumns));

    menuBar()->addMenu(USE_WIDGET(Menu,Config));

    /// add window menu
    USE_WIDGET(Menu,Window)->addAction(USE_WIDGET(Action,MemUtil));
    USE_WIDGET(Menu,Window)->addAction(USE_WIDGET(Action,WmiQuery));
    USE_WIDGET(Menu,Window)->addAction(USE_WIDGET(Action,LogDock));

    menuBar()->addMenu(USE_WIDGET(Menu,Window));

    /// add about menu
    USE_WIDGET(Menu,About)->addAction(USE_WIDGET(Action,AboutThis));
    USE_WIDGET(Menu,About)->addAction(USE_WIDGET(Action,Help));

    menuBar()->addMenu(USE_WIDGET(Menu,About));
}

void Memmon::initToolbars()
{
    /// add tools toolbar
    configToolbar(USE_WIDGET(ToolBar,Tool));

    QStringList choice ;
    choice << "1s" << "3s" << "5s";
    USE_WIDGET(ComboBox,UpdateInterval)->addItems(choice);

    USE_WIDGET(ToolBar,Tool)->addWidget(USE_WIDGET(Label,UpdateInterval));
    USE_WIDGET(ToolBar,Tool)->addWidget(USE_WIDGET(ComboBox,UpdateInterval));
    USE_WIDGET(ToolBar,Tool)->addSeparator();

    USE_WIDGET(ToolButton,Start)->setEnabled(true);
    USE_WIDGET(ToolButton,Stop)->setEnabled(false);

    USE_WIDGET(ToolButton,ShowPopup)->setCheckable(true);

    USE_WIDGET(ToolBar,Tool)->addWidget(USE_WIDGET(ToolButton,Start));
    USE_WIDGET(ToolBar,Tool)->addWidget(USE_WIDGET(ToolButton,Stop));
    USE_WIDGET(ToolBar,Tool)->addWidget(USE_WIDGET(ToolButton,Clear));
    USE_WIDGET(ToolBar,Tool)->addWidget(USE_WIDGET(ToolButton,ShowPopup));

    addToolBar(USE_WIDGET(ToolBar,Tool));

    connect(USE_WIDGET(ComboBox,UpdateInterval),SIGNAL(currentIndexChanged(int)),this,SLOT(slot_updateIntervalChanged(int)));

}


void Memmon::initLateInitVars()
{
    _selectColumnDialog = 0;
    _wmiQueryDock = 0;
    _mmDock = 0;
    _logDock = 0;
}

QAction* Memmon::createAction(const QString &strText, const QIcon &icon, const QString &strTip)
{
    QAction* act = new QAction(strText,this);
    act->setIcon(icon);
    act->setToolTip(strTip);
    connect(act,SIGNAL(triggered()),this,SLOT(slot_actionHandler()));
    return act;
}

QToolButton* Memmon::createToolButton(const QString &strText, const QIcon &icon,const QString& strTip)
{
    QToolButton* button = new QToolButton(this);
    button->setAutoRaise(true);
    button->setAutoRepeat(true);
    button->setText(strText);
    button->setIcon(icon);
    button->setToolTip(strTip);
    connect(button,SIGNAL(clicked()),this,SLOT(slot_toolbuttonHandler()));
    return button;
}

void Memmon::showSelectColumnDialog()
{
    if(_selectColumnDialog == 0)
    {
        _selectColumnDialog = new SelectColumnDialog(this);
        connect(_selectColumnDialog,SIGNAL(sig_setColumns(QStringList)),this,SLOT(slot_setColumns(QStringList)));
    }
    _selectColumnDialog->show();
}

void Memmon::showLogWindow()
{
    if(_logDock == 0)
    {
        _logDock = new LogOutputWindow(this);
        _logDock->setAction(USE_WIDGET(Action,LogDock));
        addDockWidget(Qt::BottomDockWidgetArea,_logDock);
    }

    showDock(_logDock);
}

void Memmon::showMemoryUtilityWindow()
{
    if(_mmDock == 0)
    {
        _mmDock = new MemoryMonitorDock(this);
        _mmDock->setAction(USE_WIDGET(Action,MemUtil));
        addDockWidget(Qt::BottomDockWidgetArea,_mmDock);
    }

    showDock(_mmDock);
}

void Memmon::showHelp()
{
    QMessageBox::information(this,tr("Help"),"Not implemented yet !");
}

void Memmon::showAboutThis()
{
    QMessageBox::information(this,tr("About"),MM::Text::Info_AboutThis);
}

void Memmon::showWMIQueryWindow()
{
    if(_wmiQueryDock == 0)
    {
        _wmiQueryDock = new InfoQueryDock(this);
        _wmiQueryDock->setAction(USE_WIDGET(Action,MemUtil));
        addDockWidget(Qt::BottomDockWidgetArea,_wmiQueryDock);
    }

    showDock(_wmiQueryDock);
}

void Memmon::showPopupChart()
{

}

void Memmon::configToolbar(QToolBar *toolbar)
{
    toolbar->setFixedHeight(30);
    toolbar->setMovable(false);
}

void Memmon::populateTable()
{
    for(int i = 0; i < 10; i++)
    {
        XProcessItem* newItem = new XProcessItem;
        newItem->setIcon(QIcon(":/images/process.png"));
        newItem->setName(QObject::tr("Process %1").arg(i+1));
        _processTable->addProcess(newItem);

        for(int j = 0;j < 5;j++)
        {
            XProcessItem* subItem = new XProcessItem;
            subItem->setName(QObject::tr("Sub Process %1").arg(j+1));
            subItem->setIcon(QIcon(":/images/process.png"));
            newItem->addChild(subItem);

            for(int k = 0;k < 5;k++)
            {
                XProcessItem* subSubItem = new XProcessItem;
                subSubItem->setIcon(QIcon(":/images/process.png"));
                subSubItem->setName(QObject::tr("SubSub Process %1").arg(j+1));
                subItem->addChild(subSubItem);
            }

        }
    }

}

void Memmon::showDock(QDockWidget *dock)
{
    if(dock->isVisible())
    {
        dock->hide();
    }
    else
    {
        dock->show();
    }
}

void Memmon::showStatus(const QString &strStatus)
{
    statusBar()->showMessage(strStatus,3000);
}

void Memmon::setupStatusbar()
{
    statusBar()->addWidget(USE_WIDGET(Label,Status));
    USE_WIDGET(Label,Status)->setText("Status: Stopped ");
    statusBar()->addWidget(USE_WIDGET(Widget,CpuIndicator));
    statusBar()->addWidget(USE_WIDGET(Widget,MemIndicator));
}

void Memmon::updateStatus(bool running)
{
    if(running)
    {
        USE_WIDGET(Label,Status)->setText("Status: Running ");
    }
    else
    {
        USE_WIDGET(Label,Status)->setText("Status: Stoppped ");
    }
}

void Memmon::exportContents()
{

}

void Memmon::closeEvent(QCloseEvent *)
{

}


///
/// PRIVATE SLOT FUNCTIONS
///
void Memmon::slot_toolbuttonHandler()
{
    QToolButton* who = qobject_cast<QToolButton*>(sender());

    if(who == USE_WIDGET(ToolButton,Start))
    {
        _queryManager->start();
        who->setEnabled(false);
        USE_WIDGET(ToolButton,Stop)->setEnabled(true);
        updateStatus(true);
    }

    if(who == USE_WIDGET(ToolButton,Stop))
    {
        who->setEnabled(false);
        _queryManager->stop();
        USE_WIDGET(ToolButton,Start)->setEnabled(true);
        updateStatus(false);
    }

    if(who == USE_WIDGET(ToolButton,Clear))
    {
        showStatus(MM::Text::Status_ClearHistoryData);
        _queryManager->clearHistoryData();
    }

    if(who == USE_WIDGET(ToolButton,ShowPopup))
    {
        if(USE_WIDGET(ToolButton,ShowPopup)->isChecked())
        {
            showStatus(MM::Text::Status_ShowPopup);
            _queryManager->showPopup(true);
        }
        else
        {
            showStatus(MM::Text::Status_HidePopup);
            _queryManager->showPopup(false);
        }
    }
}

void Memmon::slot_actionHandler()
{
    QAction* who = qobject_cast<QAction*>(sender());

    if(who == USE_WIDGET(Action,Exit))
    {
        close();
    }

    if(who == USE_WIDGET(Action,SelectColumns))
    {
        showSelectColumnDialog();
    }

    if(who == USE_WIDGET(Action,LogDock))
    {
        showLogWindow();
    }

    if(who == USE_WIDGET(Action,MemUtil))
    {
        showMemoryUtilityWindow();
    }

    if(who == USE_WIDGET(Action,WmiQuery))
    {
        showWMIQueryWindow();
    }

    if(who == USE_WIDGET(Action,Help))
    {
        showHelp();
    }

    if(who == USE_WIDGET(Action,AboutThis))
    {
        showAboutThis();
    }
}


void Memmon::slot_setColumns(const QStringList &columns)
{
    _varProxy.columns = columns;
    _queryManager->setColumns(columns);
}

void Memmon::slot_updateIntervalChanged(int index)
{
    if(index == 0)
    {
        _queryManager->setUpdateInterval(1000);
    }
    else if(index == 1)
    {
        _queryManager->setUpdateInterval(3000);
    }
    else if(index == 2)
    {
        _queryManager->setUpdateInterval(5000);
    }
}

void Memmon::slot_queryStopped()
{
    USE_WIDGET(ToolButton,Start)->setEnabled(true);
    USE_WIDGET(ToolButton,Stop)->setEnabled(false);
    updateStatus(false);
}
