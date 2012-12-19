#include "processtable.h"
#include <QHeaderView>
#include "mmdef.h"
#include <QDebug>
#include "queryengine.h"
#include "logutil.h"

extern std::string WChar2Ansi(LPCWSTR pwszSrc);

ProcessTable::ProcessTable(QWidget *parent) :
    QWidget(parent)
{
    initVars();
    createWidgets();
    initSettings();

    initRefreshTimer();
}

///
/// private functions
///
void ProcessTable::initVars()
{
    _refreshTimer = 0;
}

void ProcessTable::createWidgets()
{
    _mainLayout = new QVBoxLayout;
    _mainLayout->setContentsMargins(0,0,0,0);
    _table = new QTableWidget(this);
    _mainLayout->addWidget(_table);


    setLayout(_mainLayout);
}

void ProcessTable::initSettings()
{
    initTableHeader();

    _table->setAlternatingRowColors(true);

}

void ProcessTable::queryProcessInfo()
{
    QueryEngine* engine = QueryEngine::getInstance();
    connect(engine,SIGNAL(queryResultReady(QByteArray)),this,SLOT(queryInfoReady(QByteArray)));
    engine->setClass(MM::Constant::ProcessPerfClass);
    engine->startQuery(_query);
}
void ProcessTable::initRefreshTimer()
{
    if(_refreshTimer != 0)
    {
        return;
    }

    _refreshTimer = new QTimer(this);
    _refreshTimer->setInterval(MM::Constant::DefaultRefreshInterval);
    connect(_refreshTimer,SIGNAL(timeout()),this,SLOT(refresh()));
    _refreshTimer->start();
}

void ProcessTable::initTableHeader()
{
    QStringList headers = MM::Constant::ProcessHeaders.split(';');
}


void ProcessTable::makeQuery()
{
    _query.clear();
    _query << "path" << MM::Constant::ProcessPerfClass << "get";

    QStringList columns = MM::Constant::DefaultColumns;
    for(int i = 0;i < columns.size();i++)
    {
        if(columns.at(i).isEmpty())
        {
            continue;
        }
        if(i < (columns.size() - 1))
        {
            _query << columns.at(i) << ",";
        }
        else
        {
            _query << columns.at(i) ;
        }
    }
}

void ProcessTable::setupTableHeader()
{
    QStringList columns = MM::Constant::DefaultColumns;
    _table->setColumnCount(columns.size());
    _table->setHorizontalHeaderLabels(columns);
}

///
/// private slot functions
///
void ProcessTable::queryInfoReady(const QByteArray &result)
{
    setupTableHeader();

    LOG_VAR(result);

    QString strResult(result);
    QStringList lines = strResult.split('\n');
    int columnIndex = 0;
    int rowIndex = 0;
    bool firstTime = true;
    _table->setRowCount(lines.size());
    foreach(const QString& line,lines)
    {
        if(firstTime)
        {
            firstTime = false;
            continue;
        }

        columnIndex = 0;
        QString simpleLine = line.simplified();
        QStringList items = line.split(' ');
        foreach(const QString& item,items)
        {
            if(item.trimmed().isEmpty())
            {
                continue;
            }

            QTableWidgetItem* tableItem = new QTableWidgetItem;
            tableItem->setText(item);
            _table->setItem(rowIndex,columnIndex,tableItem);

            columnIndex++;
        }

        rowIndex++;
    }



}

///
/// public functions
///
void ProcessTable::refresh()
{
    makeQuery();
    queryProcessInfo();
}

void ProcessTable::setAutoRefresh(int ms)
{
    if(_refreshTimer == 0)
    {
        initRefreshTimer();
    }
    _refreshTimer->setInterval(ms);
}
