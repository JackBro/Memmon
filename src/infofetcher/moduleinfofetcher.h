#ifndef MODULEINFOFETCHER_H
#define MODULEINFOFETCHER_H

#include "basefetcher.h"

class ModuleInfoFetcher : public BaseFetcher
{
    Q_OBJECT
public:
    explicit ModuleInfoFetcher(QObject *parent = 0);
    
public:

    void setProcessName(const QString& processName);
    QString processName() const;

    void setPid(uint32_t pid);
    uint32_t pid() const;

protected:
    void run();

private:
    QString _processName;
    uint32_t _pid;
Q_SIGNALS:
    void sig_setModuleInfo(const QStringList& result);
};

#endif // MODULEINFOFETCHER_H
