#include "moduleinfofetcher.h"
#include "../logutil.h"

ModuleInfoFetcher::ModuleInfoFetcher(QObject *parent) :
    BaseFetcher(parent),_pid(0)
{
}

void ModuleInfoFetcher::run()
{
    QProcess pro;
    pro.setProcessChannelMode(QProcess::MergedChannels);
    const QString kArg1 = tr("IMAGENAME EQ %1").arg(_processName);
    const QString kArg2 = tr("PID eq %1").arg(_pid);
    const QString kProcess("tasklist");
    const QStringList kArgs = QStringList() <<"/m" << "/fi" << kArg1 << "/fi" << kArg2 <<"/fo" << "csv";
    pro.start(kProcess, kArgs);
    pro.waitForFinished();
    QString strResult(pro.readAllStandardOutput());
    QStringList resultList = strResult.split('\n');
    resultList.pop_front();
    resultList.pop_back();

    if(!resultList.isEmpty())
    {
        QStringList moduleList = resultList.at(0).split(',');
        moduleList.pop_front();
        moduleList.pop_front();

        const int kModuleCount = moduleList.size();

        if(kModuleCount > 0)
        {
            moduleList[0] = moduleList[0].right(moduleList[0].length() - 1);
            moduleList[kModuleCount - 1] = moduleList[kModuleCount -1].left(moduleList[kModuleCount - 1].length() - 2);
        }

        emit sig_setModuleInfo(moduleList);
    }
}


/*!
 * public functions
 */
void ModuleInfoFetcher::setProcessName(const QString &processName)
{
    _processName = processName;
}

QString ModuleInfoFetcher::processName() const
{
    return _processName;
}

void ModuleInfoFetcher::setPid(uint32_t pid)
{
    _pid = pid;
}

uint32_t ModuleInfoFetcher::pid() const
{
    return _pid;
}
