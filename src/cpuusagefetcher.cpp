#include "cpuusagefetcher.h"

#include <QDebug>

static const QString kProcess("wmic");
static const QStringList kQuery = QStringList() << "cpu" << "get" << "loadpercentage";

CpuUsageFetcher::CpuUsageFetcher(QObject *parent) :
    QThread(parent),_interval(1)
{
}

/*!
 * reimpl
 */
void CpuUsageFetcher::run()
{
    forever
    {
        QProcess pro;
        pro.setProcessChannelMode(QProcess::MergedChannels);
        pro.start(kProcess,kQuery);
        pro.waitForFinished();
        QString strOutput(pro.readAllStandardOutput());
        QStringList infoList = strOutput.split('\n');
        infoList.removeFirst();
        infoList.removeLast();
        infoList.removeLast();

        if(infoList.size() == 1)
        {
            QString strUsage(infoList.at(0));
            bool ok = false;
            int usage = strUsage.toInt(&ok);
            if(ok)
            {
                emit sig_setCpuUsage(usage);
            }
        }
        sleep(_interval);
    }
}

/*!
 * private slot functions
 */

/*!
 * public functions
 */
void CpuUsageFetcher::setInterval(int sec)
{
    _interval = sec;
}

int CpuUsageFetcher::interval() const
{
    return _interval;
}
