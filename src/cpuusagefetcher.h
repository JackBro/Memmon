#ifndef CPUUSAGEFETCHER_H
#define CPUUSAGEFETCHER_H

#include <QThread>
#include <QProcess>

class CpuUsageFetcher : public QThread
{
    Q_OBJECT
public:
    explicit CpuUsageFetcher(QObject *parent = 0);

public:
    void setInterval(int sec);
    int interval() const;

protected:
    void run();

private:
    int _interval; // sec

Q_SIGNALS:
    void sig_setCpuUsage(int usage);
};

#endif // CPUUSAGEFETCHER_H
