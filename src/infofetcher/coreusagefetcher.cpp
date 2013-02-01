#include "coreusagefetcher.h"

#include <QDebug>

CoreUsageFetcher::CoreUsageFetcher(QObject *parent) :
    BaseFetcher(parent)
{
    _query = QStringList() << "Path" << "Win32_PerfFormattedData_PerfOs_Processor" << "Get" << "PercentProcessorTime";
}

void CoreUsageFetcher::queryCoreCount()
{
    const QStringList query = QStringList() << "Path" << "Win32_PerfFormattedData_PerfOs_Processor" << "Get" << "Name";
    QProcess pro;
    pro.setProcessChannelMode(QProcess::MergedChannels);
    pro.start(kProcess,query);
    pro.waitForFinished();
    QString strOutput(pro.readAllStandardOutput());
    QStringList itemList = strOutput.split('\n');
    itemList.pop_back();
    itemList.pop_back();
    itemList.pop_back();
    QString strCoreCount(itemList.at(itemList.size() - 1));
    bool convertOk = false;
    int coreCount = strCoreCount.toInt(&convertOk);
    coreCount += 1;
    emit sig_setCoreCount(coreCount);
}

void CoreUsageFetcher::run()
{
    queryCoreCount();
    forever
    {
        QProcess pro;
        pro.setProcessChannelMode(QProcess::MergedChannels);
        pro.start(kProcess,query());
        pro.waitForFinished();
        QString strOutput(pro.readAllStandardOutput());
        QStringList itemList = strOutput.split('\n');
        itemList.pop_front();
        itemList.pop_back();
        itemList.pop_back();
        itemList.pop_back();

        for(int i = 0; i < itemList.size(); i++)
        {
            int usage = itemList.at(i).toInt();
            emit sig_setCoreUsage(i,usage);
        }

        sleep(interval());
    }
}
