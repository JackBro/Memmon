#include "memorymonitorwindow.h"
#include "mmdef.h"
#include "logutil.h"

#include <QMessageBox>

MemoryMonitorWindow::MemoryMonitorWindow(QWidget *parent) :
    QWidget(parent)
{
    initVars();
    initLayout();
    initSettings();
    loadMonitorColumns();
    initConnections();
}

///
/// PRIVATE FUNCS
///
void MemoryMonitorWindow::initLayout()
{
    _mainSplitter = new QSplitter(Qt::Horizontal,this);
    _mainSplitter->setHandleWidth(1);

    _leftWidget = new QWidget(this);
    _middleWidget = new QWidget(this);
    _rightWidget = new QWidget(this);

    _mainSplitter->addWidget(_leftWidget);
    _mainSplitter->addWidget(_middleWidget);
    _mainSplitter->addWidget(_rightWidget);

    _mainLayout = new QHBoxLayout(this);
    _mainLayout->setSpacing(0);
    _mainLayout->setContentsMargins(0,0,0,0);


    _leftLayout = new QVBoxLayout;
    _leftLayout->setSpacing(0);
    _leftLayout->setContentsMargins(0,0,0,0);

    _leftWidget->setLayout(_leftLayout);

    _middleLayout = new QVBoxLayout;
    _middleLayout->setSpacing(0);
    _middleLayout->setContentsMargins(0,0,0,0);
    _middleWidget->setLayout(_middleLayout);

    _rightLayout = new QVBoxLayout;
    _rightLayout->setSpacing(0);
    _rightLayout->setContentsMargins(0,0,0,0);
    _rightWidget->setLayout(_rightLayout);

    _buttonLayout = new QHBoxLayout;
    _rightLayout->addLayout(_buttonLayout);

    _processList = new QRadioList(this);
    _columnList = new QRadioList(this);

    _refreshButton = createButton(MM::Text::MMW_RefreshProcess);
    _startButton = createButton(MM::Text::MMW_StartMonitor);
    _stopButton = createButton(MM::Text::MMW_StopMonitor);
    _buttonLayout->addWidget(_refreshButton);
    _buttonLayout->addWidget(_startButton);
    _buttonLayout->addWidget(_stopButton);

    _memChart = new MemChart(this);
    _statusLabel = new QLabel(this);
    _statusLabel->setFixedHeight(MM::Constant::RowHeight);

    _mainLayout->setContentsMargins(0,0,0,0);
    _mainLayout->setSpacing(0);

    _processListLabel = new QLabel(MM::Text::Label_ProcessList,this);
    stylizeLabel(_processListLabel);
    _leftLayout->addWidget(_processListLabel);
    _leftLayout->addWidget(_processList);

    _columnLabel = new QLabel(MM::Text::Label_Columns,this);
    stylizeLabel(_columnLabel);
    _middleLayout->addWidget(_columnLabel);
    _middleLayout->addWidget(_columnList);

    _rightLayout->addWidget(_memChart);
    _rightLayout->addWidget(_statusLabel);

    _mainLayout->addWidget(_mainSplitter);

    setLayout(_mainLayout);

}

void MemoryMonitorWindow::initTimer()
{
    if(_updateTimer == 0)
    {
        _updateTimer = new QTimer(this);
        // default to 1 second
        _updateTimer->setInterval(1000);

        connect(_updateTimer,SIGNAL(timeout()),this,SLOT(slot_updateInfo()));
    }

}

void MemoryMonitorWindow::initVars()
{
    _updateTimer = 0;
    _isQueryingProcesses = false;
    _queryEngine = QueryEngine::getInstance();
    connect(_queryEngine,SIGNAL(queryResultReady(QByteArray)),this,SLOT(slot_queryResultReady(QByteArray)));
}

void MemoryMonitorWindow::initSettings()
{
    setWindowTitle(MM::Text::Title_MMW);
}

void MemoryMonitorWindow::initConnections()
{
    connect(_columnList,SIGNAL(itemClicked(QString)),this,SLOT(slot_setCurrentColumn(QString)));
    connect(_processList,SIGNAL(itemClicked(QString)),this,SLOT(slot_setCurrentProcess(QString)));
}

QPushButton* MemoryMonitorWindow::createButton(const QString &str)
{
    QPushButton* button = new QPushButton(this);
    button->setText(str);
    connect(button,SIGNAL(clicked()),this,SLOT(slot_buttonHandler()));
    return button;
}

void MemoryMonitorWindow::loadMonitorColumns()
{
    foreach(const QString& str,MM::Constant::MonitorColumns)
    {
        _columnList->addItem(str);
    }
    _columnList->stop();
}

void MemoryMonitorWindow::refreshProcessList()
{
    updateStatus(tr("Updating process list ..."));
    connect(_queryEngine,SIGNAL(queryResultReady(QByteArray)),this,SLOT(slot_queryResultReady(QByteArray)));
    _memChart->clear();
    if(_updateTimer && _updateTimer->isActive())
    {
        _updateTimer->stop();
    }

    QStringList arguments;
    arguments << "PATH" << MM::Constant::ProcessPerfClass << "GET" << MM::Constant::MonitorRefreshColumns.at(0)
              << "," << MM::Constant::MonitorRefreshColumns.at(1) << MM::Constant::OutputForamt;

    _queryEngine->startQuery(arguments);

    _isQueryingProcesses = true;

}


void MemoryMonitorWindow::startMonitor()
{
    if(_strProcessName.isEmpty() || _columnName.isEmpty())
    {
        QMessageBox::warning(this,tr("Warning"),tr("Cannot start monitor : either process name or column name is empty !"));

        return ;
    }

    updateStatus(tr("Running ..."));
    connect(_queryEngine,SIGNAL(queryResultReady(QByteArray)),this,SLOT(slot_queryResultReady(QByteArray)));
    QStringList arguments;
    arguments << "PATH" << MM::Constant::ProcessPerfClass  << "Where" << tr("name='%1'").arg(_strProcessName) << "GET" << _columnName;
    _queryEngine->startQuery(arguments);

    _isQueryingProcesses = false;

    QString strMemChartCaption;
    strMemChartCaption = tr("%1:%2").arg(_strProcessName).arg(_columnName);
    _memChart->setCaption(strMemChartCaption);

    initTimer();
    _updateTimer->start();

}

void MemoryMonitorWindow::stopMonitor()
{
    updateStatus(tr("Stop."));
    if(_updateTimer && _updateTimer->isActive())
    {
        _updateTimer->stop();
    }
}

void MemoryMonitorWindow::stylizeLabel(QLabel *label)
{
    QString style("font:20px bold;background-color:rgb(233,233,233);color:black;");
    label->setStyleSheet(style);
}

void MemoryMonitorWindow::updateStatus(const QString &strStatus)
{
    _statusLabel->setText(strStatus);
}

///
/// PRIVATE SLOT FUNCS
///
void MemoryMonitorWindow::slot_buttonHandler()
{
    QPushButton* who = qobject_cast<QPushButton*>(sender());

    if(who == _refreshButton)
    {
        refreshProcessList();
        return;
    }

    if(who == _startButton)
    {
        startMonitor();
        return;
    }

    if(who == _stopButton)
    {
        stopMonitor();
        return;
    }
}
void MemoryMonitorWindow::slot_setCurrentProcess(const QString& strProcessName)
{
    if(strProcessName != _strProcessName)
    {
        _memChart->clear();
    }

    QStringList nameList = strProcessName.split(':');
    _strProcessName = nameList.at(0);
}

void MemoryMonitorWindow::slot_setCurrentColumn(const QString& strColumn)
{
    if(_columnName != strColumn)
    {
        _memChart->clear();
    }

    _columnName = strColumn;
}

void MemoryMonitorWindow::slot_queryResultReady(const QByteArray &result)
{
    QString strResult(result);
    QStringList lines = strResult.split('\n');

    if(_isQueryingProcesses)
    {
        lines.pop_front();
        lines.pop_front();

        _processList->clear();

        for(int i = 0;i < lines.size() - 1;i++)
        {
            QString strLine = lines.at(i);
            QStringList resultList = strLine.split(',');
            resultList.pop_front(); // remove node info

            _processList->addItem(tr("%1:%2").arg(resultList.at(0).trimmed()).arg(resultList.at(1).trimmed()));
        }

        if(_updateTimer && _updateTimer->isActive())
        {
            _updateTimer->stop();
        }
    }
    else
    {

        lines.pop_front();
        QStringList resultList = strResult.split('\n');
        QString strValue = resultList.at(1);
        bool ok = false;
        int value = strValue.toInt(&ok);
        if(ok)
        {
            _memChart->addValue(value);
        }
    }

    disconnect(_queryEngine,SIGNAL(queryResultReady(QByteArray)),this,SLOT(slot_queryResultReady(QByteArray)));

}

void MemoryMonitorWindow::slot_updateInfo()
{
    startMonitor();
}
