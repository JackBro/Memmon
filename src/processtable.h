#ifndef PROCESSTABLE_H
#define PROCESSTABLE_H

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <windows.h>
#include <tlhelp32.h>
#include <QTimer>

class ProcessTable : public QWidget
{
    Q_OBJECT
public:
    explicit ProcessTable(QWidget *parent = 0);
    
public Q_SLOTS:
    void refresh();
    void setAutoRefresh(int ms);


private:
    void initVars();
    void createWidgets();
    void initSettings();
    void queryProcessInfo();
    void initRefreshTimer();
    void initTableHeader();
    void makeQuery();
    void setupTableHeader();

private:
    QTableWidget* _table;
    QVBoxLayout* _mainLayout;
    QTimer* _refreshTimer;

    QStringList _query;

private Q_SLOTS:
    void queryInfoReady(const QByteArray& result);

};

#endif // PROCESSTABLE_H
