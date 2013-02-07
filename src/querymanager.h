#ifndef QUERYMANAGER_H
#define QUERYMANAGER_H

#include <QObject>
#include <QTimer>

#include "xprocesstable/xprocesstable.h"
#include "queryengine.h"

class SelectColumnDialog;
class QueryManager : public QObject
{
    Q_OBJECT
public:
    explicit QueryManager(QObject *parent = 0);
    
public:
    void setColumnDialog(SelectColumnDialog* pScd);
    void setTable(XProcessTable* table);
    void setUpdateInterval(int ms);
    void setColumns(const QStringList& columns);
    void start();
    void stop();

    bool isRunning();
    void clearHistoryData();
    void showPopup(bool show);

    void setQueryEngine(const QString& strQueryEngine);
    QString queryEngine() const;

private:
    void initVars();
    void getPidColumnIndex(const QString& str);
    void notifyQueryError();

private:

    QStringList _columns;
    SelectColumnDialog* _selectColumnDialog;
    XProcessTable* _table;
    QueryEngine* _queryEngine;
    QTimer* _updateTimer;
    QStringList _headerList;
    QMap<uint32_t,XProcessItem*> _processItems;

    uint32_t _pidColumnIndex;
    uint32_t _nameColumnIndex;

    QVector<uint32_t> _pids;
    QVector<uint32_t> _deadPids;
    bool _firstRun;

private Q_SLOTS:
    void slot_doQuery();
    void slot_queryResultReady(const QByteArray& results);
    void slot_removeProcess(uint32_t pid);

Q_SIGNALS:
    void sig_queryStopped();


};

#endif // QUERYMANAGER_H
