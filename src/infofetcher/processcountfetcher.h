#ifndef PROCESSCOUNTFETCHER_H
#define PROCESSCOUNTFETCHER_H

#include "basefetcher.h"

class ProcessCountFetcher : public BaseFetcher
{
    Q_OBJECT
public:
    explicit ProcessCountFetcher(QObject *parent = 0);

protected:
    void run();

Q_SIGNALS:
    void sig_setTotalProcessCount(int cnt);

};

#endif // PROCESSCOUNTFETCHER_H
