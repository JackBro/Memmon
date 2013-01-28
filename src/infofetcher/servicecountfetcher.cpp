#include "servicecountfetcher.h"

ServiceCountFetcher::ServiceCountFetcher(QObject *parent) :
    BaseFetcher(parent)
{
    _query = QStringList() << "Service" << "where" << "State='Running'" << "Get" << "Caption";
}

void ServiceCountFetcher::run()
{
    forever
    {
        int count = getQueryResult();
        emit sig_setRunningServiceCount(count);
        sleep(interval());
    }
}

