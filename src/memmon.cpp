#include "memmon.h"
#include "mmdef.h"

#include <QSettings>
#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QFileDialog>

#undef USE_WIDGET
#define USE_WIDGET(WIDGET_NAME,WIDGET_ENUM) _uiProxy->get ## WIDGET_NAME(MmUiProxy :: WIDGET_NAME ## _ ## WIDGET_ENUM)

#undef MM_INIT_VAR
#define MM_INIT_VAR(VAR,VALUE) VAR = VALUE

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
}

void Memmon::initConnections()
{
    PYProg* cpuProg = (PYProg*)USE_WIDGET(Widget,CpuIndicator);
    connect(cpuProg,SIGNAL(sig_mousePressed()),this,SLOT(slot_showCpuUsageHistory()));

    PYProg* memProg = (PYProg*)USE_WIDGET(Widget,MemIndicator);
    connect(memProg,SIGNAL(sig_mousePressed()),this,SLOT(slot_showMemUsageHistory()));

    connect(((QToolButton*)USE_WIDGET(ToolButton,GeneralInfo)),SIGNAL(clicked()),this,SLOT(slot_showGeneralInfo()));

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

    statusBar()->addPermanentWidget(USE_WIDGET(ToolButton,GeneralInfo));

    QFont boldFont;
    boldFont.setBold(true);
    USE_WIDGET(Label,TotalProcessCount)->setFont(boldFont);

    statusBar()->addPermanentWidget(USE_WIDGET(Label,TotalProcessCount));
    statusBar()->addPermanentWidget(USE_WIDGET(Widget,CpuIndicator));
    statusBar()->addPermanentWidget(USE_WIDGET(Widget,MemIndicator));
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
    outStream << _processTable->contents();

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

void Memmon::slot_showCpuUsageHistory()
{
    if(USE_WIDGET(Widget,CpuUsageHistory)->isVisible())
    {
        USE_WIDGET(Widget,CpuUsageHistory)->hide();
    }
    else
    {
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
