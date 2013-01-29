#ifndef USAGEINFOPAD_H
#define USAGEINFOPAD_H

#include <QWidget>
#include <QVBoxLayout>

#include "ctrl/perfchart.h"
#include "ctrl/pyhistory.h"

class UsageInfoPad : public QWidget
{
    Q_OBJECT
public:
    explicit UsageInfoPad(QWidget *parent = 0);
    
public Q_SLOTS:
    void addWidget(QWidget* w);
    void setCoreCount(int count);
    void addCoreUsage(int index, int usage);

protected:
    void closeEvent(QCloseEvent *);

private:
    void setupLayout();
    void initSettings();

private:
    QVBoxLayout* _layout;
    PerfChart* _perfChart;

Q_SIGNALS:
    void sig_closed();
    
};

#endif // USAGEINFOPAD_H
