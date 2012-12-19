#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTimer>
#include "memchart.h"

class TestWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TestWidget(QWidget *parent = 0);
    

private:
    QVBoxLayout* mainLayout;
    MemChart* chart;
    QTimer* updateTimer;

private Q_SLOTS:
    void stuffValue();
};

#endif // TESTWIDGET_H
