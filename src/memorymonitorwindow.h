#ifndef MEMORYMONITORWINDOW_H
#define MEMORYMONITORWINDOW_H

#include <QWidget>
#include <QTabWidget>
#include <QListWidget>
#include <QRadioButton>
#include <QGroupBox>
#include <QPushButton>
#include <QSplitter>
#include <QTimer>
#include <QLabel>

#include "memchart.h"
#include "queryengine.h"
#include "qradiolist.h"

class MemoryMonitorWindow : public QWidget
{
    Q_OBJECT
private:
    explicit MemoryMonitorWindow(QWidget *parent = 0);
    
public:
    void setUpdateInterval(int ms);

private:

    void initLayout();
    void initTimer();
    void initVars();
    void initSettings();
    void initConnections();
    QPushButton* createButton(const QString& str);
    void loadMonitorColumns();
    void refreshProcessList();
    void startMonitor();
    void stopMonitor();
    void stylizeLabel(QLabel* label);
    void updateStatus(const QString& strStatus);

private:

    QLabel* _processListLabel;
    QLabel* _columnLabel;

    QHBoxLayout* _mainLayout;
    QSplitter* _mainSplitter;
    QWidget* _leftWidget;
    QWidget* _middleWidget;
    QWidget* _rightWidget;
    QVBoxLayout* _leftLayout;
    QVBoxLayout* _middleLayout;
    QVBoxLayout* _rightLayout;
    QHBoxLayout* _buttonLayout;

    QPushButton* _refreshButton;
    QRadioList* _processList;

    QPushButton* _startButton;
    QPushButton* _stopButton;
    QRadioList* _columnList;

    MemChart* _memChart;
    QLabel* _statusLabel;
    QueryEngine* _queryEngine;

    QString _strProcessName;
    QString _columnName;

    bool _isQueryingProcesses;

    QTimer* _updateTimer;


private Q_SLOTS:
    void slot_buttonHandler();
    void slot_setCurrentProcess(const QString& strProcessName);
    void slot_setCurrentColumn(const QString& strColumn);
    void slot_queryResultReady(const QByteArray& result);
    void slot_updateInfo();


    friend class MemoryMonitorDock;
};

#endif // MEMORYMONITORWINDOW_H
