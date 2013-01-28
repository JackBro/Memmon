#include "processcountfetcher.h"

ProcessCountFetcher::ProcessCountFetcher(QObject *parent) :
    BaseFetcher(parent)
{
    _query = QStringList() << "OS" << "Get" << "NumberOfProcesses";
}

void ProcessCountFetcher::run()
{
    forever
    {
        QProcess pro;
        pro.setProcessChannelMode(QProcess::MergedChannels);
        pro.start(kProcess,query());
        pro.waitForFinished();
        QString strOutput(pro.readAllStandardOutput());
        QStringList infoList = strOutput.split('\n');
        infoList.removeFirst();
        infoList.removeLast();
        infoList.removeLast();

        if(infoList.size() == 1)
        {
            QString strCount(infoList.at(0));
            bool ok = false;
            int count = strCount.toInt(&ok);
            if(ok)
            {
                emit sig_setTotalProcessCount(count);
            }
        }
        sleep(interval());
    }
}
