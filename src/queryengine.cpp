#include "queryengine.h"
#include "mmdef.h"
#include "logutil.h"
#include <windows.h>
#include <QApplication>

#ifndef SAFE_DELETE
#define SAFE_DELETE(OBJECT) if(OBJECT){ delete OBJECT; OBJECT = NULL;}
#endif

//#define USE_THREAD

QueryEngine* QueryEngine::_instance = NULL;
QueryEngine::QueryEngine(QObject *parent) :
    QObject(parent)
{
    _class = MM::Constant::ProcessPerfClass;
    _isQueryColumns = false;
}

///
/// private slot functions
///

void QueryEngine::finishQuery()
{
    QProcess* worker = qobject_cast<QProcess*>(sender());
    worker->deleteLater();
    _queryResult = worker->readAllStandardOutput();

    if(worker->error() == QProcess::UnknownError)
    {
        emit getQueryError(_queryResult);
//        return;
    }

    processQueryResult();
}

void QueryEngine::setQueryResult(const QByteArray &result)
{
    _queryResult = result;
    processQueryResult();
}

/// *!
/// private utility functions
/// *!
void QueryEngine::queryColumns()
{

    QStringList queryColumnsStatement;
    _class = _class.toStdString().c_str();
    queryColumnsStatement << "PATH" << _class << "GET" << "/?";

#ifdef USE_THREAD

    QueryThread* thread = new QueryThread(this);
    connect(thread,SIGNAL(setQueryResult(QByteArray)),this,SLOT(setQueryResult(QByteArray)));
    thread->setQuery(queryColumnsStatement);
    thread->start();


#else
    QProcess* worker = new QProcess(this);
    setupWorkerHandler(worker);
    worker->start(MM::Constant::CMD_Engine,queryColumnsStatement);
    QApplication::processEvents();

#endif
}

void QueryEngine::parseColumns()
{
    _columns.clear();
    QString strResult(_queryResult);
    QStringList resultList = strResult.split('\n');

    bool startPushing = false;
    foreach(const QString& strLine,resultList)
    {
        if(strLine.contains( MM::Constant::TopSeparator))
        {
            startPushing = true;
            continue;
        }

        if(startPushing)
        {
            QStringList lines = strLine.split(' ');

            if(lines.size() > 0 && !lines.at(0).trimmed().isEmpty())
            {
             _columns.push_back(lines.at(0));
            }

            if(strLine.trimmed().isEmpty())
            {
                break;
            }
        }
    }
    emit getColumns(_columns);
}

void QueryEngine::setupWorkerHandler(QProcess *worker)
{
    worker->setProcessChannelMode(QProcess::MergedChannels);
    connect(worker,SIGNAL(readyReadStandardOutput()),this,SLOT(finishQuery()));
}

///
/// public functions
///
QueryEngine* QueryEngine::getInstance()
{
#ifdef USE_SINGLE_ENGINE
//    if(_instance == NULL)
//    {
//        _instance = new QueryEngine;
//    }
//    return _instance;
#else
    QueryEngine* newEngine = new QueryEngine;
    return newEngine;
#endif
}

void QueryEngine::startQuery(const QStringList &query)
{
    _isQueryColumns = false;
    _query = query;
    QProcess* worker = new QProcess(this);
    setupWorkerHandler(worker);
    worker->start(MM::Constant::CMD_Engine,query);
}

QStringList QueryEngine::query() const
{
    return _query;
}

QByteArray QueryEngine::getQueryResult() const
{
    return _queryResult;
}

void QueryEngine::startQueryColumns(const QString &className)
{
    setClass(className);
    startQueryColumns();
}

void QueryEngine::startQueryColumns()
{
    _isQueryColumns = true;
    queryColumns();
}

void QueryEngine::processQueryResult()
{
    if(_isQueryColumns)
    {
        parseColumns();
    }
    else
    {
        emit queryResultReady(_queryResult);
    }
}

void QueryEngine::setClass(const QString &Class)
{
    _class = Class;
}

QString QueryEngine::getClass() const
{
    return _class;
}
