#ifndef CPUUSAGEFETCHER_H
#define CPUUSAGEFETCHER_H

#include <QThread>
#include <QProcess>

#include "basefetcher.h"

class CpuUsageFetcher : public BaseFetcher
{
    Q_OBJECT
public:
    explicit CpuUsageFetcher(QObject *parent = 0);

protected:
    void run();

Q_SIGNALS:
    void sig_setCpuUsage(int usage);
};

#endif // CPUUSAGEFETCHER_H
