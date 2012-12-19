#include "waitingwidget.h"

#include <QPainter>
#include <QPaintEvent>
#include <QTimer>
#include "QProgressIndicator.h"
#include <QLabel>
#include <QDesktopWidget>

namespace WW
{
    const QColor BG_Start = QColor(241,241,241);
    const QColor BG_Stop = QColor(205,205,205);
    const QColor Pen = QColor(185,185,185);

    const int ElapsedTime = 3000; // 3 seconds
    const int ExtraSpace = 10;
    const int RoundRadius = 16;
    const int PenWidth = 1;
}

#ifndef SAFE_DELETE
#define SAFE_DELETE(Object) if(Object){ delete Object; Object = NULL;}
#endif // SAFE_DELETE

/**********************************************/
/*!  WaitingLabel                             */
/**********************************************/
WaitingLabel* WaitingLabel::_instance = NULL;

WaitingLabel::WaitingLabel(QWidget *parent):QWidget(parent),_parent(parent)
{

    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    setWindowModality(Qt::ApplicationModal);

    _parent = 0;

    _label = new QLabel(this);
    _label->setWordWrap(true);
    _label->setStyleSheet("color:black;font:bold;");

    _indicator = new QProgressIndicator(this);
    _indicator->setAnimationDelay(100);
    _indicator->startAnimation();

    initTimer();
}


void WaitingLabel::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    QPainter painter(this);
    drawBackground(&painter);
}

void WaitingLabel::resizeEvent(QResizeEvent *e)
{
    calcGeo();

    QWidget::resizeEvent(e);
}

void WaitingLabel::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);

    QDesktopWidget desktop;
    QRect desktopRect = desktop.screenGeometry();

    QPoint movePoint(desktopRect.center().x() - width()/2,desktopRect.center().y() - height()/2);
    move(movePoint);
}

void WaitingLabel::drawBackground(QPainter *painter)
{
    painter->save();
    painter->setRenderHints(QPainter::HighQualityAntialiasing|QPainter::Antialiasing);

    painter->setPen(QPen(WW::Pen,WW::PenWidth));

    QLinearGradient bgGradient(rect().topLeft(),rect().bottomLeft());
    bgGradient.setColorAt(0.0,WW::BG_Start);
    bgGradient.setColorAt(1.0,WW::BG_Stop);
    painter->setBrush(bgGradient);

    painter->drawRoundedRect(rect(),WW::RoundRadius,WW::RoundRadius);

    painter->restore();
}


void WaitingLabel::calcGeo()
{
//    if(_parent)
//    {
//        int thisWidth = _parent->size().width()/3;
//        int thisHeight = thisWidth * 0.68;
//        setFixedSize(thisWidth,thisHeight);

//    }

    qreal indicatorWidth = (width() - 2 * WW::ExtraSpace)/4;
    qreal initX = WW::ExtraSpace;

    // layout progress indicator
    QPoint indicatorPoint(initX,height()/2 - indicatorWidth/2);
    _indicator->resize(QSize(indicatorWidth,indicatorWidth));
    _indicator->move(indicatorPoint);

    // layout label
    QPoint labelPoint(indicatorWidth + 2 * WW::ExtraSpace,initX);
    QSize labelSize(width() - 3 * WW::ExtraSpace - indicatorWidth,height() - 2 * WW::ExtraSpace);
    _label->resize(labelSize);
    _label->move(labelPoint);
}

void WaitingLabel::initTimer()
{
    _timer = new QTimer(this);
    _timer->setInterval(WW::ElapsedTime);
    connect(_timer,SIGNAL(timeout()),this,SLOT(hide()));
}

WaitingLabel* WaitingLabel::instance()
{
    if(_instance == NULL)
    {
        _instance = new WaitingLabel();
    }
    return _instance;
}

void WaitingLabel::setText(const QString &strText)
{
    _label->setText(strText);
}

void WaitingLabel::setParent(QWidget *parent)
{
    _parent = parent;
}

void WaitingLabel::done()
{
    initTimer();
    _timer->start();
}

/**********************************************/
/*! WaitingWidget                            !*/
/**********************************************/

WaitingWidget::WaitingWidget(QWidget *parent) :
    QWidget(parent)
{

}

WaitingWidget::~WaitingWidget()
{

}

///
/// private utility functions
///


///
///
///
void WaitingWidget::setParent(QWidget* parent)
{
    WaitingLabel::instance()->setParent(parent);
}

void WaitingWidget::showText(const QString& strText)
{
    WaitingLabel::instance()->setText(strText);
    WaitingLabel::instance()->show();
}

void WaitingWidget::show()
{
    WaitingLabel::instance()->show();
}

void WaitingWidget::hide()
{
    WaitingLabel::instance()->close();
}

void WaitingWidget::done()
{
    WaitingLabel::instance()->done();
}
