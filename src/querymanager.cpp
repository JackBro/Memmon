#include "querymanager.h"
#include "logutil.h"
#include "mmdef.h"
#include "xprocesstable/datachart.h"
#include "selectcolumndialog.h"

#include <QMessageBox>

QueryManager::QueryManager(QObject *parent) :
    QObject(parent)
{
    initVars();
}

///
/// PRIVATE UTILITY FUNCTIONS
///
void QueryManager::initVars()
{
    _selectColumnDialog = 0;
    _queryEngine = QueryEngine::getInstance();
    connect(_queryEngine,SIGNAL(queryResultReady(QByteArray)),this,SLOT(slot_queryResultReady(QByteArray)));

    _updateTimer = new QTimer(this);
    _updateTimer->setInterval(1000);
    connect(_updateTimer,SIGNAL(timeout()),this,SLOT(slot_doQuery()));

    _columns.clear();
    foreach(const QString& strCol,MM::Constant::DefaultColumns)
    {
        _columns << strCol;
    }


    _pidColumnIndex = -1;
    _nameColumnIndex = -1;

}

void QueryManager::getPidColumnIndex(const QString &str)
{
    QStringList colList = str.split(MM::Constant::ColumnSeparator);

    if(colList.size() > 0 && !colList.at(0).contains("node",Qt::CaseInsensitive))
    {
        notifyQueryError();
        return;
    }

    colList.pop_front();


    // because the column order returned by the query is not the same
    // as we set , so we reset columns by query results

    _columns.clear();

    for(int i = 0;i < colList.size();i++)
    {
        _columns.push_back(colList.at(i).trimmed());
        if(colList.at(i).trimmed().contains(_pidColumnName,Qt::CaseInsensitive))
        {
            _pidColumnIndex = i;
        }

        if(colList.at(i).trimmed().contains(MM::Constant::Name,Qt::CaseInsensitive))
        {
            _nameColumnIndex = i;
        }
    }

}

void QueryManager::notifyQueryError()
{
//    QMessageBox::warning(0,"Error","Query error:invalid query,\nplease reset the query columns!");
//    _updateTimer->stop();
//    emit sig_queryStopped();
    if(_selectColumnDialog)
    {
        _selectColumnDialog->fire_setColumns();
    }
}
void QueryManager::notifyPidColumnNameChanged(const QString& strQueryEngine)
{
    if(strQueryEngine.toLower() == "win32_process")
    {
        _pidColumnName = "ProcessID";
    }
    else
    {
        _pidColumnName = "IDProcess";
    }
}

void QueryManager::stopQuery()
{
    _updateTimer->stop();
    emit sig_queryStopped();
}

///
/// PUBLIC FUNCTIONS
///
void QueryManager::setTable(XProcessTable *table)
{
    _table = table;
//    connect(_table,SIGNAL(sig_removeProcess(uint32_t)),this,SLOT(slot_removeProcess(uint32_t)));
    _table->setColumns(_columns);
    _table->setAutoAdjust(true);
}

void QueryManager::setColumnDialog(SelectColumnDialog *pScd)
{
    _selectColumnDialog = pScd;
}

void QueryManager::setUpdateInterval(int ms)
{
    if(_updateTimer)
    {
        _updateTimer->setInterval(ms);
    }
}

void QueryManager::setColumns(const QStringList &columns)
{
    _columns = columns;
    _table->clear();
    _table->setColumns(columns);
    _table->setAutoAdjust(true);

    _processItems.clear();
    _pids.clear();

    // if the columns changed update immediately
    slot_doQuery();
}

void QueryManager::start()
{
    _firstRun = true;
    slot_doQuery();
    _updateTimer->start();
}

void QueryManager::stop()
{
    _updateTimer->stop();
}

bool QueryManager::isRunning()
{
    return _updateTimer->isActive();
}

void QueryManager::clearHistoryData()
{
    QMap<uint32_t,XProcessItem*>::iterator iter = _processItems.begin();

    while(++iter != _processItems.end())
    {
        XProcessItem* item = iter.value();
        if(item)
        {
            item->clearHistoryData();
        }
    }
}

void QueryManager::showPopup(bool show)
{
    if(!show)
    {
        DataChart::instance()->setVisible(show);
    }

    QMap<uint32_t,XProcessItem*>::iterator iter = _processItems.begin();

    while(++iter != _processItems.end())
    {
        XProcessItem* item = iter.value();
        if(item)
        {
            item->showPopup(show);
        }
    }
}

void QueryManager::setQueryEngine(const QString &strQueryEngine)
{
    if(_queryEngine->queryEngine() != strQueryEngine)
    {
        _queryEngine->setQueryEngine(strQueryEngine);
        notifyPidColumnNameChanged(strQueryEngine);
        stopQuery();
    }
}

QString QueryManager::queryEngine() const
{
    return _queryEngine->queryEngine();
}

///
/// PRIVATE SLOT FUNCTIONS
///
void QueryManager::slot_doQuery()
{

    QStringList arguments;
    arguments << "PATH" << _queryEngine->queryEngine() <<  "GET";

    for(int i = 0;i < _columns.size();i++)
    {
        if(i >= 0 && i < _columns.size() - 1)
        {
            arguments << _columns.at(i) << ",";
        }
        else
        {
            arguments << _columns.at(i);
        }
    }

    arguments << MM::Constant::OutputForamt;
    _queryEngine->startQuery(arguments);
}

void QueryManager::slot_queryResultReady(const QByteArray &results)
{
    QString strResults(results);

    if(strResults.contains("invalid query"),Qt::CaseInsensitive)
    {
        notifyQueryError();
        return;
    }


    QStringList resultList = strResults.split('\n');
    resultList.pop_back();

    // the first line is header
    if(resultList.size() > 2)
    {
        QString firstLine = resultList.at(1);
        if(!firstLine.contains("node",Qt::CaseInsensitive))
        {
            int i = 0;
        }
        getPidColumnIndex(firstLine);
    }

    _table->sortByColumns(_columns);

    if(_pidColumnIndex != -1)
    {
        for(int i = 2;i < resultList.size();i++)
        {
            QString strLineResult = resultList.at(i);
            QStringList innerResultList = strLineResult.split(MM::Constant::ColumnSeparator);
            innerResultList.pop_front();

            if(innerResultList.isEmpty())
            {
                continue;
            }

            uint32_t pid = innerResultList.at(_pidColumnIndex).toInt();
            QString processName = innerResultList.at(_nameColumnIndex);

            if(!_processItems.contains(pid))
            {
                XProcessItem* processItem = new XProcessItem((QWidget*)_table->d());
                processItem->setName(processName);
                processItem->setPidColumnIndex(_pidColumnIndex);
                processItem->setNameColumnIndex(_nameColumnIndex);
                processItem->createWidgetByColumns(_columns);
                processItem->setPid(innerResultList.at(_pidColumnIndex).toUInt());
                processItem->updateInfo(innerResultList);
                _table->addProcess(processItem);
                _processItems.insert(pid,processItem);

                _deadPids.push_back(pid);

            }
            else
            {
                XProcessItem* item = _processItems[pid];
                item->updateInfo(innerResultList);

                int deadIndex = _deadPids.indexOf(pid);
                if(deadIndex != -1)
                {
                    _deadPids.remove(deadIndex);
                }
            }
        }
    }

    if(!_firstRun)
    {
        for(int i = 0;i < _deadPids.size();i++)
        {
            XProcessItem* item = _processItems.value(_deadPids.at(i));

            if(item)
            {

                _processItems.remove(_deadPids.at(i));
                _table->deleteProcess(_deadPids.at(i));
                _deadPids.remove(i);

                delete item;
                item = 0;

            }
        }
    }
    _firstRun = false;

}

void QueryManager::slot_removeProcess(uint32_t pid)
{
    if(_processItems.contains(pid))
    {
        _processItems.remove(pid);
    }

}
