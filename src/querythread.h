#ifndef QUERYTHREAD_H
#define QUERYTHREAD_H

#include <QThread>
#include <QProcess>

class QueryThread : public QThread
{
    Q_OBJECT
public:
    explicit QueryThread(QObject *parent = 0);

public:
    void setQuery(const QStringList& query);
    QStringList query() const;

    
protected:
    void run();

private:
    QStringList _query;
    QByteArray  _result;
    QProcess* _worker;

    QObject* _parent;

private Q_SLOTS:
    void finishQuery();

Q_SIGNALS:
    void setQueryResult(const QByteArray& queryResult);
};

#endif // QUERYTHREAD_H
