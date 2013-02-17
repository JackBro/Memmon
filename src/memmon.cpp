#include "memmon.h"
#include "mmdef.h"
#include "util.h"
#include "logutil.h"

#include <QSettings>
#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopWidget>
#include <QApplication>

#include "../infofetcher/moduleinfofetcher.h"

#undef USE_WIDGET
#define USE_WIDGET(WIDGET_NAME,WIDGET_ENUM) _uiProxy->get ## WIDGET_NAME(MmUiProxy :: WIDGET_NAME ## _ ## WIDGET_ENUM)

#undef MM_INIT_VAR
#define MM_INIT_VAR(VAR,VALUE) VAR = VALUE

void setWindowCenter(QWidget *window,
        double widthScale = 1, double heightScale = 1)
{
    double ws = widthScale < 0.1 ? 0.1 : widthScale;
    double hs = heightScale < 0.1 ? 0.1 : heightScale;
    ws = ws > 1 ? 1 : ws;
    hs = hs > 1 ? 1 : hs;
    QRect rect = QApplication::desktop()->availableGeometry();
    int iTitleHeight = window->style()->pixelMetric(QStyle::PM_TitleBarHeight);
    window->setGeometry(rect.width() * (1 - ws) / 2,
            rect.height() * (1 - hs) / 2 + iTitleHeight,
            rect.width() * ws,
            rect.height() * hs - iTitleHeight);
}

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
    initUsageFetcher();
    initConnections();
    restoreSettings();
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
    MM_INIT_VAR(_generalInfoPad,0);
    MM_INIT_VAR(_infoPadContainer,0);
    MM_INIT_VAR(_usageInfoPad,0);
    MM_INIT_VAR(_coreUsageFetcher,0);

    _uiProxy = new MmUiProxy(this);
    _queryManager = new QueryManager(this);
    connect(_queryManager,SIGNAL(sig_queryStopped()),this,SLOT(slot_queryStopped()));
}

void Memmon::initSettings()
{
    setWindowTitle(tr("Memory Monitor"));
    showMaximized();

    PYHistory* cpuHistory = (PYHistory*)USE_WIDGET(Widget,CpuUsageHistory);
    cpuHistory->setWindowTitle("CPU Usage Monitor");
    cpuHistory->setCaption(tr("CPU Usage History"));
    cpuHistory->setChannelCount(1);
    cpuHistory->setChannelCaption(0,tr("CPU Usage"));
    cpuHistory->setWindowFlags(Qt::Drawer);
    cpuHistory->hide();

    PYHistory* memHistory = (PYHistory*)USE_WIDGET(Widget,MemUsageHistory);
    memHistory->setWindowTitle("Memory Usage Monitor");
    memHistory->setCaption(tr("Memory Usage History"));
    memHistory->setChannelCount(1);
    memHistory->setChannelCaption(0,tr("Memory Usage"));
    memHistory->setWindowFlags(Qt::Drawer);
    memHistory->hide();

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
    USE_WIDGET(Menu,Config)->addMenu(USE_WIDGET(Menu,QueryEngine));

    QActionGroup* queryEngineActGroup = new QActionGroup(this);
    queryEngineActGroup->addAction(USE_WIDGET(Action,Win32_Process));
    queryEngineActGroup->addAction(USE_WIDGET(Action,Win32_PerfFormattedData_PerfProc_Process));

    USE_WIDGET(Menu,QueryEngine)->addAction(USE_WIDGET(Action,Win32_Process));
    USE_WIDGET(Menu,QueryEngine)->addAction(USE_WIDGET(Action,Win32_PerfFormattedData_PerfProc_Process));

    menuBar()->addMenu(USE_WIDGET(Menu,Config));

    /// add tools menu
    menuBar()->addMenu(USE_WIDGET(Menu,Tools));
    USE_WIDGET(Menu,Tools)->addAction(USE_WIDGET(Action,TakeSnapshot));

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
    USE_WIDGET(ToolBar,Tool)->addAction(USE_WIDGET(Action,SelectColumns));
    USE_WIDGET(ToolBar,Tool)->addSeparator();

    USE_WIDGET(ToolButton,Start)->setEnabled(true);
    USE_WIDGET(ToolButton,Stop)->setEnabled(false);

    USE_WIDGET(ToolButton,ShowPopup)->setCheckable(true);

    USE_WIDGET(ToolBar,Tool)->addWidget(USE_WIDGET(ToolButton,Start));
    USE_WIDGET(ToolBar,Tool)->addWidget(USE_WIDGET(ToolButton,Stop));
    USE_WIDGET(ToolBar,Tool)->addWidget(USE_WIDGET(ToolButton,Clear));
    USE_WIDGET(ToolBar,Tool)->addWidget(USE_WIDGET(ToolButton,ShowPopup));
    USE_WIDGET(ToolBar,Tool)->addAction(USE_WIDGET(Action,TakeSnapshot));
    USE_WIDGET(ToolBar,Tool)->addAction(USE_WIDGET(Action,ShowModuleList));
    USE_WIDGET(ToolBar,Tool)->addWidget(USE_WIDGET(Label,SearchEditPrompt));
    USE_WIDGET(ToolBar,Tool)->addWidget(USE_WIDGET(Widget,SearchEdit));
    USE_WIDGET(ToolBar,Tool)->addWidget(USE_WIDGET(Label,DataCountPrompt));
    USE_WIDGET(ToolBar,Tool)->addWidget(USE_WIDGET(SpinBox,DataCount));


    addToolBar(USE_WIDGET(ToolBar,Tool));

    connect(USE_WIDGET(ComboBox,UpdateInterval),SIGNAL(currentIndexChanged(int)),this,SLOT(slot_updateIntervalChanged(int)));
    connect(USE_WIDGET(Widget,SearchEdit),SIGNAL(textChanged(QString)),this,SLOT(slot_find(QString)));
    connect(USE_WIDGET(SpinBox,DataCount),SIGNAL(valueChanged(int)),this,SLOT(slot_setDataCount(int)));
}

void Memmon::initUsageFetcher()
{
    _cpuUsageFetcher = new CpuUsageFetcher(this);
    connect(_cpuUsageFetcher,SIGNAL(sig_setCpuUsage(int)),this,SLOT(slot_updateCpuUsage(int)));
    _cpuUsageFetcher->start();

    _memUsageFetcher = new MemoryUsageFetcher(this);
    connect(_memUsageFetcher,SIGNAL(sig_setMemoryUsage(int)),this,SLOT(slot_updateMemUsage(int)));
    _memUsageFetcher->start();

    _processCountFetcher = new ProcessCountFetcher(this);
    connect(_processCountFetcher,SIGNAL(sig_setTotalProcessCount(int)),this,SLOT(slot_setTotalProcessCount(int)));
    _processCountFetcher->start();

    _serviceCountFetcher = new ServiceCountFetcher(this);
    connect(_serviceCountFetcher,SIGNAL(sig_setRunningServiceCount(int)),this,SLOT(slot_setRunningServiceCount(int)));
    _serviceCountFetcher->start();

    _driverCountFetcher = new DriverCountFetcher(this);
    connect(_driverCountFetcher,SIGNAL(sig_setRunningDriverCount(int)),this,SLOT(slot_setRunningDriverCount(int)));
    _driverCountFetcher->start();

}

void Memmon::initConnections()
{
    PYProg* cpuProg = (PYProg*)USE_WIDGET(Widget,CpuIndicator);
    connect(cpuProg,SIGNAL(sig_mousePressed()),this,SLOT(slot_showCpuUsageHistory()));

    PYProg* memProg = (PYProg*)USE_WIDGET(Widget,MemIndicator);
    connect(memProg,SIGNAL(sig_mousePressed()),this,SLOT(slot_showMemUsageHistory()));

    connect(((QToolButton*)USE_WIDGET(ToolButton,GeneralInfo)),SIGNAL(clicked()),this,SLOT(slot_showGeneralInfo()));

    connect(USE_WIDGET(Action,Win32_Process),SIGNAL(triggered()),this,SLOT(slot_switchQueryEngine()));
    connect(USE_WIDGET(Action,Win32_PerfFormattedData_PerfProc_Process),SIGNAL(triggered()),this,SLOT(slot_switchQueryEngine()));
    connect(USE_WIDGET(Action,TakeSnapshot),SIGNAL(triggered()),this,SLOT(slot_takeSnapshot()));

    connect(_processTable, SIGNAL(sig_processClicked(QString)), this, SLOT(slot_setCurrentProcessName(QString)));
    connect(_processTable, SIGNAL(sig_processClicked(uint32_t)), this , SLOT(slot_setCurrentPid(uint32_t)));
}

void Memmon::initLateInitVars()
{
    MM_INIT_VAR(_selectColumnDialog,0);
    MM_INIT_VAR(_wmiQueryDock,0);
    MM_INIT_VAR(_mmDock,0);
    MM_INIT_VAR(_logDock,0);
    MM_INIT_VAR(_moduleInfoDock,0);
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
        _selectColumnDialog->setQueryEngine(_queryManager->queryEngine());
        _queryManager->setColumnDialog(_selectColumnDialog);
        _selectColumnDialog->setAction(USE_WIDGET(Action,SelectColumns));
        _selectColumnDialog->reload();
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

void Memmon::showModuleInfoWindow()
{
    if(_moduleInfoDock == 0)
    {
        _moduleInfoDock = new ModuleInfoDock(this);
        _moduleInfoDock->setAction(USE_WIDGET(Action,ShowModuleList));
        addDockWidget(Qt::RightDockWidgetArea, _moduleInfoDock);
    }
    showDock(_moduleInfoDock);
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

    statusBar()->addPermanentWidget(USE_WIDGET(ToolButton,GeneralInfo));

    QFont boldFont;
    boldFont.setBold(true);
    USE_WIDGET(Label,TotalProcessCount)->setFont(boldFont);
    slot_addUsageWidgets();
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
    QString strExportFile = QFileDialog::getSaveFileName(this,tr("Export Contents"),".",QString("Text Files(*.log)"));
    if(strExportFile.isEmpty())
    {
        return;
    }

    QFile exportFile(strExportFile);
    if(!exportFile.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this,tr("Warning"),tr("Failed to write contents to \"%1\"").arg(strExportFile));
        return;
    }

    QTextStream outStream(&exportFile);
    QString strContent(_processTable->contents());
    QMessageBox::information(this,"FYI",strContent);
//    outStream << strContent;

}

void Memmon::addToInfoPad(int categoryIndex, const QByteArray &output)
{
    QString strOutput(output);
    QStringList strLines = strOutput.split('\n');
    strLines.removeFirst();
    strLines.removeLast();
    Q_ASSERT(strLines.size() == 2);

    QString captionLine(strLines.at(0));
    QString valueLine(strLines.at(1));

    QStringList captionList = captionLine.split(',');
    QStringList valueList = valueLine.split(',');

    captionList.removeFirst();
    valueList.removeFirst();

    //    Q_ASSERT(captionList.size() == valueList.size());

    for(int i = 0; i < captionList.size(); i++)
    {
        _generalInfoPad->AddItem(categoryIndex,qMakePair(captionList.at(i), valueList.at(i).isEmpty() ? "N/A" : valueList.at(i)));
    }
}

void Memmon::showUsageInfoPad()
{
    if(_usageInfoPad == 0)
    {
        _usageInfoPad = new UsageInfoPad(this);
        connect(_usageInfoPad,SIGNAL(sig_closed()),this,SLOT(slot_addUsageWidgets()));

        _coreUsageFetcher = new CoreUsageFetcher(this);
        connect(_coreUsageFetcher,SIGNAL(sig_setCoreCount(int)),_usageInfoPad,SLOT(setCoreCount(int)));
        connect(_coreUsageFetcher,SIGNAL(sig_setCoreUsage(int,int)),_usageInfoPad,SLOT(addCoreUsage(int,int)));
        connect(_usageInfoPad,SIGNAL(sig_safeQuit()),this,SLOT(slot_safeQuit()));
        _coreUsageFetcher->start();

    }

    _usageInfoPad->addWidget(USE_WIDGET(Label,TotalProcessCount));
    _usageInfoPad->addWidget(USE_WIDGET(Label,RunningServiceCount));
    _usageInfoPad->addWidget(USE_WIDGET(Label,RunningDriverCount));
    _usageInfoPad->addWidget(USE_WIDGET(Widget,CpuIndicator));
    _usageInfoPad->addWidget(USE_WIDGET(Widget,MemIndicator));
    _usageInfoPad->done();

    QDesktopWidget desktop;
    QRect deskRect = desktop.availableGeometry();
    QPoint movePoint(deskRect.width() - _usageInfoPad->width() - MM::Constant::ExtraSpace,deskRect.height() - _usageInfoPad->height() - MM::Constant::ExtraSpace);
    _usageInfoPad->move(movePoint);

    _usageInfoPad->show();
}

void Memmon::stopInfoFetcher(QThread *fetcher)
{
    if(fetcher)
    {
        fetcher->terminate();
        fetcher->deleteLater();
    }
}

void Memmon::saveSettings()
{
    QByteArray windowSettings = saveState();
    Util::SettingMgr::WriteSetting2ByteA(MM::Text::Key_WindowStates,windowSettings);

    Util::SettingMgr::WriteSetting(MM::Text::Key_QueryEngine,_queryManager->queryEngine());
}

void Memmon::restoreSettings()
{
    restoreState(Util::SettingMgr::ReadSetting2ByteA(MM::Text::Key_WindowStates));

    QString strQueryEngine = Util::SettingMgr::ReadSetting(MM::Text::Key_QueryEngine);
    if(strQueryEngine.isEmpty() || strQueryEngine.toLower() == MM::Text::Win32_Process.toLower())
    {
        USE_WIDGET(Action,Win32_Process)->setChecked(true);
        _queryManager->setQueryEngine(USE_WIDGET(Action,Win32_Process)->text());
    }
    else
    {
        USE_WIDGET(Action,Win32_PerfFormattedData_PerfProc_Process)->setChecked(true);
        _queryManager->setQueryEngine(USE_WIDGET(Action,Win32_PerfFormattedData_PerfProc_Process)->text());

    }
}

void Memmon::closeEvent(QCloseEvent *e)
{
    showUsageInfoPad();
    e->ignore();
    hide();
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
        exit(0);
    }

    if(who == USE_WIDGET(Action,SelectColumns))
    {
        showSelectColumnDialog();
    }

    if(who == USE_WIDGET(Action,ShowModuleList))
    {
        showModuleInfoWindow();
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

    if(who == USE_WIDGET(Action,Export))
    {
        exportContents();
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

void Memmon::slot_updateCpuUsage(int usage)
{
    ((PYProg*)USE_WIDGET(Widget,CpuIndicator))->setValue(usage);

    PYHistory* cpuHistory = (PYHistory*)USE_WIDGET(Widget,CpuUsageHistory);
    cpuHistory->addChannelData(0,usage);
}

void Memmon::slot_updateMemUsage(int usage)
{
    ((PYProg*)USE_WIDGET(Widget,MemIndicator))->setValue(usage);

    PYHistory* memHistory = (PYHistory*)USE_WIDGET(Widget,MemUsageHistory);
    memHistory->addChannelData(0,usage);
}

void Memmon::slot_setTotalProcessCount(int count)
{
    ((QLabel*)(USE_WIDGET(Label,TotalProcessCount)))->setText(tr("Number of Processes: %1").arg(count));
}

void Memmon::slot_setRunningServiceCount(int count)
{
    ((QLabel*)(USE_WIDGET(Label,RunningServiceCount)))->setText(tr("Running Services: %1").arg(count));
}

void Memmon::slot_setRunningDriverCount(int count)
{
    ((QLabel*)(USE_WIDGET(Label,RunningDriverCount)))->setText(tr("Running Drivers: %1").arg(count));
}

void Memmon::slot_showCpuUsageHistory()
{
    if(USE_WIDGET(Widget,CpuUsageHistory)->isVisible())
    {
        USE_WIDGET(Widget,CpuUsageHistory)->hide();
    }
    else
    {
        setWindowCenter(USE_WIDGET(Widget,CpuUsageHistory),0.4,0.5);
        USE_WIDGET(Widget,CpuUsageHistory)->show();
    }
}

void Memmon::slot_showMemUsageHistory()
{
    if(USE_WIDGET(Widget,MemUsageHistory)->isVisible())
    {
        USE_WIDGET(Widget,MemUsageHistory)->hide();
    }
    else
    {
        setWindowCenter(USE_WIDGET(Widget,MemUsageHistory),0.4,0.5);
        USE_WIDGET(Widget,MemUsageHistory)->show();
    }
}

void Memmon::slot_showGeneralInfo()
{
    if(_generalInfoPad == 0)
    {
        _infoPadContainer = new QScrollArea(this);
        _infoPadContainer->setWidgetResizable(true);
        _infoPadContainer->setWindowFlags(Qt::Drawer);
        _infoPadContainer->setWindowTitle(tr("General Info"));
        _infoPadContainer->resize(QSize(400,400));

        _generalInfoPad = new PYInfoPad(this);
        _infoPadContainer->setWidget(_generalInfoPad);

        const QString kProcess("WMIC");
        const QStringList kCpuQuery = QStringList() << "Cpu" << "List" << "/Format:csv";
        const QStringList kCsQuery = QStringList() << "ComputerSystem" << "List" << "/Format:csv";

        QProcess pro;
        pro.setProcessChannelMode(QProcess::MergedChannels);

        // query cpu info
        _generalInfoPad->AddCategory(tr("CPU Info"));
        pro.start(kProcess,kCpuQuery);
        pro.waitForFinished();
        addToInfoPad(0,pro.readAllStandardOutput());

        // query computer system info
        _generalInfoPad->AddCategory(tr("Computer System Info"));
        pro.start(kProcess,kCsQuery);
        pro.waitForFinished();
        addToInfoPad(1,pro.readAllStandardOutput());
    }

    _infoPadContainer->show();
}

void Memmon::slot_addUsageWidgets()
{
    statusBar()->addPermanentWidget(USE_WIDGET(Label,TotalProcessCount));
    statusBar()->addPermanentWidget(USE_WIDGET(Label,RunningServiceCount));
    statusBar()->addPermanentWidget(USE_WIDGET(Label,RunningDriverCount));
    statusBar()->addPermanentWidget(USE_WIDGET(Widget,CpuIndicator));
    statusBar()->addPermanentWidget(USE_WIDGET(Widget,MemIndicator));

    if(_usageInfoPad && _usageInfoPad->isVisible())
    {
        _usageInfoPad->hide();
    }

    show();
}

void Memmon::slot_safeQuit()
{
    saveSettings();
    stopInfoFetcher(_cpuUsageFetcher);
    stopInfoFetcher(_memUsageFetcher);
    stopInfoFetcher(_processCountFetcher);
    stopInfoFetcher(_serviceCountFetcher);
    stopInfoFetcher(_driverCountFetcher);
    stopInfoFetcher(_coreUsageFetcher);
    qApp->exit(0);
}

void Memmon::slot_switchQueryEngine()
{
    QAction* who = qobject_cast<QAction*>(sender());
    QString strQueryEngine = who->text();

    if(_queryManager->queryEngine() != strQueryEngine)
    {
        _queryManager->setQueryEngine(strQueryEngine);

        showSelectColumnDialog();
        _selectColumnDialog->setQueryEngine(strQueryEngine);
        _selectColumnDialog->reload();
    }
}

void Memmon::slot_takeSnapshot()
{
    QString strSaveImage = QFileDialog::getSaveFileName(this,"Save Snapshot ...",".","PNG Files(*.png)");
    if(strSaveImage.isEmpty())
    {
        return;
    }

    QPixmap pixmap = QPixmap::grabWidget(this,0,0,width(),height());
    pixmap.save(strSaveImage);
}

void Memmon::slot_find(const QString &expr)
{
    if(_processTable)
    {
        _processTable->find(expr);
    }
    _queryManager->setFindExpr(expr);
}

void Memmon::slot_setDataCount(int dataCnt)
{
    if(_processTable)
    {
        _processTable->setDataCount(dataCnt);
    }
}

void Memmon::slot_setCurrentProcessName(const QString &processName)
{
    _varProxy.currentProcessName = processName;
    slot_getProcessModuleInfo(processName);
}

void Memmon::slot_setCurrentPid(uint32_t pid)
{
    _varProxy.currentPid = pid;
}

void Memmon::slot_getProcessModuleInfo(const QString &processName)
{
    LOG_VAR(processName);
    ModuleInfoFetcher* fetcher = new ModuleInfoFetcher(this);
    connect(fetcher, SIGNAL(sig_setModuleInfo(QStringList)), this, SLOT(slot_parseModuleInfo(QStringList)));
    fetcher->setProcessName(_varProxy.currentProcessName);
    fetcher->setPid(_varProxy.currentPid);
    fetcher->start();
}

void Memmon::slot_parseModuleInfo(const QStringList &moduleList)
{
//    ModuleInfoFetcher* who = qobject_cast<ModuleInfoFetcher*>(sender());
//    who->deleteLater();
    if(_moduleInfoDock)
    {
        _moduleInfoDock->setModuleList(moduleList);
    }
}
