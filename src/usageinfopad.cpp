#include "usageinfopad.h"

#include <QApplication>

static const int kMargin = 0;

UsageInfoPad::UsageInfoPad(QWidget *parent) :
    QWidget(parent),_quitButton(NULL)
{
    setupLayout();
    initSettings();
}

/*!
 * private utility functions
 */
void UsageInfoPad::initVars()
{
    _quitButton = createButton(tr("Quit"));
    connect(_quitButton,SIGNAL(clicked()),this,SIGNAL(sig_safeQuit()));
}

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

QPushButton* UsageInfoPad::createButton(const QString &strText, const QIcon &icon)
{
    QPushButton* button = new QPushButton(this);
    button->setText(strText);
    button->setIcon(icon);
    return button;
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

void UsageInfoPad::done()
{
    if(_quitButton != NULL)
    {
        return;
    }

    initVars();
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(_quitButton);
    _layout->addLayout(buttonLayout);
}
