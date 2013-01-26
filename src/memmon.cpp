#include "memmon.h"
#include "mmdef.h"

#include <QSettings>
#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>

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
    _fileMenu = new QMenu(MM::Text::Menu_File,this);

    _exitAct = createAction(MM::Text::File_Exit);
    _exitAct->setShortcut(QKeySequence::Quit);

    _fileMenu->addAction(_exitAct);
    menuBar()->addMenu(_fileMenu);

    /// add config menu
    _configMenu = new QMenu(MM::Text::Menu_Config,this);
    _selectColumnAct = createAction(MM::Text::Config_SelectColumn);
    _configMenu->addAction(_selectColumnAct);

    menuBar()->addMenu(_configMenu);

    /// add window menu
    _windowMenu = new QMenu(MM::Text::Menu_Window,this);

    _memUtilAct = createAction(MM::Text::Window_MemoryUtility);
    _memUtilAct->setCheckable(true);

    _logDockAct = createAction(MM::Text::Window_Log);
    _logDockAct->setCheckable(true);

    _wmiQueryAct = createAction(MM::Text::Window_WMIQuery);
    _wmiQueryAct->setCheckable(true);

    _windowMenu->addAction(_memUtilAct);
    _windowMenu->addAction(_wmiQueryAct);
    _windowMenu->addAction(_logDockAct);

    menuBar()->addMenu(_windowMenu);

    /// add about menu
    _aboutMenu = new QMenu(MM::Text::Menu_About,this);
    _aboutThisAct = createAction(MM::Text::About_This);
    _helpAct = createAction(MM::Text::About_Help);

    _aboutMenu->addAction(_aboutThisAct);
    _aboutMenu->addAction(_helpAct);

    menuBar()->addMenu(_aboutMenu);
}

void Memmon::initToolbars()
{
    /// add tools toolbar
    _toolBar = new QToolBar(MM::Text::ToolBar_Tools,this);
    configToolbar(_toolBar);
    _updateIntervalLabel = new QLabel(MM::Text::UpdateIntervalLabel,this);
    _updateIntervalCombo = new QComboBox(this);

    QStringList choice ;
    choice << "1s" << "3s" << "5s";

    _updateIntervalCombo->addItems(choice);

    _toolBar->addWidget(_updateIntervalLabel);
    _toolBar->addWidget(_updateIntervalCombo);
    _toolBar->addSeparator();

    _startButton = createToolButton(MM::Text::TB_StartQuery,QIcon(":/images/start_query.png"),MM::Text::TB_StartQuery);
    _stopButton = createToolButton(MM::Text::TB_StopQuery,QIcon(":/images/stop_query.png"),MM::Text::TB_StopQuery);
    _stopButton->setEnabled(false);
    _startButton->setEnabled(true);

    _clearButton = createToolButton(MM::Text::TB_ClearHistoryData,QIcon(":/images/clear.png"),MM::Text::TB_ClearHistoryData);

    _showPopupButton = createToolButton(MM::Text::TB_ShowPopupChart,QIcon(":/images/chart.png"),MM::Text::TB_ShowPopupChart);
    _showPopupButton->setCheckable(true);

    _toolBar->addWidget(_startButton);
    _toolBar->addWidget(_stopButton);
    _toolBar->addWidget(_clearButton);
    _toolBar->addWidget(_showPopupButton);

    addToolBar(_toolBar);

    connect(_updateIntervalCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_updateIntervalChanged(int)));

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
        _logDock->setAction(_logDockAct);
        addDockWidget(Qt::BottomDockWidgetArea,_logDock);
    }

    showDock(_logDock);
}

void Memmon::showMemoryUtilityWindow()
{
    if(_mmDock == 0)
    {
        _mmDock = new MemoryMonitorDock(this);
        _mmDock->setAction(_memUtilAct);
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
        _wmiQueryDock->setAction(_wmiQueryAct);
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
    statusBar()->addWidget(_uiProxy.getLabel(MmUiProxy::Label_Status));
    _uiProxy.getLabel(MmUiProxy::Label_Status)->setText("Status: Stopped ");
    statusBar()->addWidget(_uiProxy.getWidget(MmUiProxy::Widget_CpuIndicator));
    statusBar()->addWidget(_uiProxy.getWidget(MmUiProxy::Widget_MemIndicator));
}

void Memmon::updateStatus(bool running)
{
    if(running)
    {
        _uiProxy.getLabel(MmUiProxy::Label_Status)->setText("Status: Running ");
    }
    else
    {
        _uiProxy.getLabel(MmUiProxy::Label_Status)->setText("Status: Stoppped ");
    }
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

    if(who == _startButton)
    {
        _queryManager->start();
        who->setEnabled(false);
        _stopButton->setEnabled(true);
        updateStatus(true);
    }

    if(who == _stopButton)
    {
        who->setEnabled(false);
        _queryManager->stop();
        _startButton->setEnabled(true);
        updateStatus(false);
    }

    if(who == _clearButton)
    {
        showStatus(MM::Text::Status_ClearHistoryData);
        _queryManager->clearHistoryData();
    }

    if(who == _showPopupButton)
    {
        if(_showPopupButton->isChecked())
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

    if(who == _exitAct)
    {
        close();
    }

    if(who == _selectColumnAct)
    {
        showSelectColumnDialog();
    }

    if(who == _logDockAct)
    {
        showLogWindow();
    }

    if(who == _memUtilAct)
    {
        showMemoryUtilityWindow();
    }

    if(who == _wmiQueryAct)
    {
        showWMIQueryWindow();
    }

    if(who == _helpAct)
    {
        showHelp();
    }

    if(who == _aboutThisAct)
    {
        showAboutThis();
    }
}


void Memmon::slot_setColumns(const QStringList &columns)
{
    _columns = columns;
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
    _startButton->setEnabled(true);
    _stopButton->setEnabled(false);
    updateStatus(false);
}
