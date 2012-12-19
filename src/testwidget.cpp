#include "testwidget.h"

TestWidget::TestWidget(QWidget *parent) :
    QWidget(parent)
{
    mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    chart = new MemChart(this);
    chart->setCaption("svchost.exe");

    mainLayout->addWidget(chart);
    setLayout(mainLayout);

    updateTimer = new QTimer(this);
    updateTimer->setInterval(500);
    connect(updateTimer,SIGNAL(timeout()),this,SLOT(stuffValue()));
    updateTimer->start();
}

void TestWidget::stuffValue()
{
    int value = rand()%10000;
    chart->addValue(value);
}
