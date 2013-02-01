#ifndef USAGEINFOPAD_H
#define USAGEINFOPAD_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>

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
    void done();

protected:
    void closeEvent(QCloseEvent *);

private:
    void initVars();
    void setupLayout();
    void initSettings();

    QPushButton* createButton(const QString& strText, const QIcon& icon = QIcon());

private:
    QVBoxLayout* _layout;
    PerfChart* _perfChart;
    QPushButton* _quitButton;


Q_SIGNALS:
    void sig_closed();
    
};

#endif // USAGEINFOPAD_H
