#ifndef MEMORYUSAGEFETCHER_H
#define MEMORYUSAGEFETCHER_H

#include <QThread>
#include <QProcess>

class MemoryUsageFetcher : public QThread
{
    Q_OBJECT
public:
    explicit MemoryUsageFetcher(QObject *parent = 0);

public:
    void setInterval(int sec);
    int interval() const;

protected:
    void run();

private:
    double getResult(const QByteArray& result);

private:
    int _interval;

Q_SIGNALS:
    void sig_setMemoryUsage(int usage);
};

#endif // MEMORYUSAGEFETCHER_H
