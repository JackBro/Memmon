#ifndef DRIVERCOUNTFETCHER_H
#define DRIVERCOUNTFETCHER_H

#include "basefetcher.h"

class DriverCountFetcher : public BaseFetcher
{
    Q_OBJECT
public:
    explicit DriverCountFetcher(QObject *parent = 0);

protected:
    void run();

Q_SIGNALS:
    void sig_setRunningDriverCount(int count);
};

#endif // DRIVERCOUNTFETCHER_H
