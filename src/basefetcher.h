#ifndef BASEFETCHER_H
#define BASEFETCHER_H

#include <QThread>
#include <QStringList>
#include <QProcess>

class BaseFetcher : public QThread
{
    Q_OBJECT
public:
    explicit BaseFetcher(QObject *parent = 0);

public:
    void setInterval(int sec);
    int interval() const;

    void setQuery(const QStringList& query);
    QStringList query() const;

    static QString kProcess;

protected:
    int _interval;
    QStringList _query;


};

#endif // BASEFETCHER_H
