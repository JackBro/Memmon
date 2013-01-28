#include "usageinfopad.h"

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
    setLayout(_layout);
}

void UsageInfoPad::initSettings()
{
    setWindowFlags(Qt::Drawer | Qt::WindowStaysOnTopHint);
    setWindowTitle(tr("System Info"));
}

/*!
 * public functions
 */
void UsageInfoPad::addWidget(QWidget *w)
{
    _layout->addWidget(w);
}

/*!
 * reimpl
 */
void UsageInfoPad::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent(e);
    emit sig_closed();
}
