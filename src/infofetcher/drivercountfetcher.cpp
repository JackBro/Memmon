#include "drivercountfetcher.h"

DriverCountFetcher::DriverCountFetcher(QObject *parent) :
    BaseFetcher(parent)
{
    _query = QStringList() << "SysDriver" << "Where" << "State='Running'" << "Get" << "Caption";
}

void DriverCountFetcher::run()
{
    forever
    {
        int count = getQueryResult();
        emit sig_setRunningDriverCount(count);
        sleep(interval());
    }
}
