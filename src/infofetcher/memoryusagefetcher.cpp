#include "memoryusagefetcher.h"

static const QStringList kQueryAvailable = QStringList() << "Path" << "Win32_PerfRawData_PerfOS_Memory" << "Get" << "AvailableBytes";
static const QStringList kQueryTotal = QStringList() << "ComputerSystem" << "Get" << "TotalPhysicalMemory";

MemoryUsageFetcher::MemoryUsageFetcher(QObject *parent) :
    BaseFetcher(parent)
{

}

/*!
 * reimpl
 */
void MemoryUsageFetcher::run()
{
    forever
    {
        QProcess pro;
        pro.setProcessChannelMode(QProcess::MergedChannels);

        // get availabel memory
        pro.start(kProcess,kQueryAvailable);
        pro.waitForFinished();
        double availableMem = getResult(pro.readAllStandardOutput());

        // get total memory
        pro.start(kProcess,kQueryTotal);
        pro.waitForFinished();
        double totalMem = getResult(pro.readAllStandardOutput());

        if(totalMem >= 0)
        {
            // calculate memory usage
            int usage = (totalMem - availableMem)/totalMem * 100;
            emit sig_setMemoryUsage(usage);
        }

        sleep(_interval);
    }
}

/*!
 *
 */
double MemoryUsageFetcher::getResult(const QByteArray &result)
{
    QString strOutput(result);
    QStringList infoList = strOutput.split('\n');
    infoList.removeFirst();
    infoList.removeLast();
    infoList.removeLast();

    if(infoList.size() == 1)
    {
        QString strUsage(infoList.at(0).trimmed());
        bool ok = false;
        double usage = strUsage.toDouble(&ok);
        if(ok)
        {
            return usage;
        }
    }
    return -1;
}
