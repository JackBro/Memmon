#ifndef COREUSAGEFETCHER_H
#define COREUSAGEFETCHER_H

#include "basefetcher.h"

class CoreUsageFetcher : public BaseFetcher
{
    Q_OBJECT
public:
    explicit CoreUsageFetcher(QObject *parent = 0);

protected:
    void run();

private:
    void queryCoreCount();

Q_SIGNALS:
    void sig_setCoreCount(int count);
    void sig_setCoreUsage(int coreIndex,int usage);
};

#endif // COREUSAGEFETCHER_H
