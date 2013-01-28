#include "basefetcher.h"

QString BaseFetcher::kProcess = QString("WMIC");

BaseFetcher::BaseFetcher(QObject *parent) :
    QThread(parent),_interval(1)
{
}

/*!
 * public functions
 */
void BaseFetcher::setInterval(int sec)
{
    _interval = sec;
}

int BaseFetcher::interval() const
{
    return _interval;
}

void BaseFetcher::setQuery(const QStringList &query)
{
    _query = query;
}

QStringList BaseFetcher::query() const
{
    return _query;
}

int BaseFetcher::getQueryResult()
{
    QProcess pro;
    pro.setProcessChannelMode(QProcess::MergedChannels);
    pro.start(kProcess,query());
    pro.waitForFinished();
    QString strOutput(pro.readAllStandardOutput());
    QStringList infoList = strOutput.split('\n');
    infoList.removeFirst();
    infoList.removeLast();
    infoList.removeLast();
    return infoList.size();
}
