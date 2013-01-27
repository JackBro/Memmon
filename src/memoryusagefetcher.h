#ifndef MEMORYUSAGEFETCHER_H
#define MEMORYUSAGEFETCHER_H

#include "basefetcher.h"

class MemoryUsageFetcher : public BaseFetcher
{
    Q_OBJECT
public:
    explicit MemoryUsageFetcher(QObject *parent = 0);

protected:
    void run();

private:
    double getResult(const QByteArray& result);

Q_SIGNALS:
    void sig_setMemoryUsage(int usage);
};

#endif // MEMORYUSAGEFETCHER_H
