#ifndef QUERYENGINE_H
#define QUERYENGINE_H

#include <QObject>
#include <QProcess>
#include "querythread.h"

class QueryEngine : public QObject
{
    Q_OBJECT
private:
    explicit QueryEngine(QObject *parent = 0);
    
public:

    static QueryEngine* getInstance();
    void startQuery(const QStringList& query);
    QStringList query() const;

    QByteArray getQueryResult() const;

    void startQueryColumns(const QString& className);


    void setClass(const QString& Class);
    QString getClass() const;


private:
    void queryColumns();
    void parseColumns();
    void setupWorkerHandler(QProcess* worker);
    // this function will fire up the column fetching procedure
    // and the results will be passed out by signal "getColumns"
    void startQueryColumns();
    void processQueryResult();

private:

    static QueryEngine* _instance;

    QByteArray _queryResult;
    QString _class;

    QStringList _query;
    QStringList _columns;

    bool _isQueryColumns;

private Q_SLOTS:
    void finishQuery();
    void setQueryResult(const QByteArray& result);

Q_SIGNALS:
    void queryResultReady(const QByteArray& queryResult);
    void getColumns(const QStringList& columns);
    void getQueryError(const QByteArray& errorInfo);
};

#endif // QUERYENGINE_H
