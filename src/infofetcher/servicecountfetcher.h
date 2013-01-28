#ifndef SERVICECOUNTFETCHER_H
#define SERVICECOUNTFETCHER_H

#include "basefetcher.h"

class ServiceCountFetcher : public BaseFetcher
{
    Q_OBJECT
public:
    explicit ServiceCountFetcher(QObject *parent = 0);

protected:
    void run();

Q_SIGNALS:
    void sig_setRunningServiceCount(int count);

};

#endif // SERVICECOUNTFETCHER_H
