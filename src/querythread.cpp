#include "querythread.h"
#include "mmdef.h"
#include "logutil.h"

QueryThread::QueryThread(QObject *parent) :
    QThread(parent),_parent(parent)
{
//    _worker = new QProcess(parent);
//    connect(_worker,SIGNAL(readyReadStandardOutput()),this,SLOT(finishQuery()));
}

///
/// reimplemented functions
///
void QueryThread::run()
{
//    _worker->start(MM::Constant::CMD_Engine,_query);
    QProcess* worker = new QProcess(_parent);
    worker->start(MM::Constant::CMD_Engine,_query);
}


///
/// private slot functions
///
void QueryThread::finishQuery()
{
    QProcess* worker = qobject_cast<QProcess*>(sender());
//    QByteArray queryResult = _worker->readAllStandardOutput();
    QByteArray queryResult = worker->readAllStandardOutput();
    emit setQueryResult(queryResult);
}

///
/// public functions
///
void QueryThread::setQuery(const QStringList &query)
{
    _query = query;
}

QStringList QueryThread::query() const
{
    return _query;
}

