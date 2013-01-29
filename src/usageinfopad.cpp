#include "usageinfopad.h"

static const int kMargin = 0;

UsageInfoPad::UsageInfoPad(QWidget *parent) :
    QWidget(parent)
{
    setupLayout();
    initSettings();
}

/*!
 * private utility functions
 */
void UsageInfoPad::setupLayout()
{
    _layout = new QVBoxLayout;
    _layout->setContentsMargins(kMargin, kMargin, kMargin, kMargin);
    _perfChart = new PerfChart(this);
    _perfChart->setCaption(tr("Core Utilizations"));
    _perfChart->setHorizontalTitle(QString());
    _perfChart->setVerticalTitle(QString());
    _layout->addWidget(_perfChart);
    setLayout(_layout);
}

void UsageInfoPad::initSettings()
{
    setWindowFlags(Qt::Drawer | Qt::WindowStaysOnTopHint);
    setWindowTitle(tr("System Info"));
}

/*!
 * reimpl
 */
void UsageInfoPad::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent(e);
    emit sig_closed();
}

/*!
 * public functions
 */
void UsageInfoPad::addWidget(QWidget *w)
{
    _layout->addWidget(w);
}

void UsageInfoPad::setCoreCount(int count)
{
    _perfChart->setChannelCount(count);

    for(int i = 0; i < count; i++)
    {
        _perfChart->setChannelText(i,tr("Core %1").arg(i));
//        _perfChart->setChannelColor(i,QColor(rand()%255, rand()%255, rand()%255));
    }
}

void UsageInfoPad::addCoreUsage(int index, int usage)
{
    _perfChart->addChannelData(index,usage);
}
