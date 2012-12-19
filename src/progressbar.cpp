#include "progressbar.h"
#include <QDebug>

ProgressBar::ProgressBar(QWidget *parent) :
    QWidget(parent)
{
    _min = 0;
    _max = 100;
    _value = 0;
    _textAlignment = PB::Center;

    setFixedHeight(PB::Constant::Height);
}

///
///
///
void ProgressBar::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    drawBackground(&painter);
    drawForeground(&painter);
    drawText(&painter);
}

void ProgressBar::drawBackground(QPainter *painter)
{
    painter->save();
    setupBrush(painter,PB::Color::Normal_Start,PB::Color::Normal_Middle,PB::Color::Normal_Stop);
    painter->drawRoundedRect(rect(),PB::Constant::RoundRadius,PB::Constant::RoundRadius);
    painter->restore();
}

void ProgressBar::drawForeground(QPainter *painter)
{
    qreal xPos = (qreal)_value/(_max - _min) * width();
    qDebug() << xPos;
    QRectF theRect(rect().topLeft(),QPointF(xPos,height()));
    painter->save();
    setupBrush(painter,PB::Color::Busy_Start,PB::Color::Busy_Middle,PB::Color::Busy_Stop);
    painter->drawRoundedRect(theRect,PB::Constant::RoundRadius,PB::Constant::RoundRadius);
    painter->restore();
}

void ProgressBar::drawText(QPainter *painter)
{
    painter->save();
    QFont textFont;
    textFont.setBold(true);
    painter->setFont(textFont);

    QRectF textRect(QPointF(PB::Constant::TextExtraSpace,0),QPointF(width() - PB::Constant::TextExtraSpace,height()));

    switch(_textAlignment)
    {
    case PB::Left:
        painter->drawText(textRect,tr("%1%").arg(_value),Qt::AlignLeft|Qt::AlignVCenter);
        break;
    case PB::Center:
        painter->drawText(textRect,tr("%1%").arg(_value),Qt::AlignVCenter|Qt::AlignHCenter);
        break;
    case PB::Right:
        painter->drawText(textRect,tr("%1%").arg(_value),Qt::AlignRight|Qt::AlignVCenter);
        break;
    }

    painter->restore();
}

void ProgressBar::setupBrush(QPainter *painter, const QColor &startColor, const QColor &middleColor, const QColor &stopColor)
{
    painter->setPen(QColor(165,165,165));
    QLinearGradient bgGradient(rect().topLeft(),rect().bottomLeft());
    bgGradient.setColorAt(0.0,startColor);
    bgGradient.setColorAt(0.2,middleColor);
    bgGradient.setColorAt(1.0,stopColor);
    painter->setBrush(bgGradient);
}

///
/// private utility functions
///

///
/// public interfaces
///
void ProgressBar::setRange(int min,int max)
{
    _min = min;
    _max = max;
    update();
}

void ProgressBar::setValue(int value)
{
    _value = value;
    update();
}

int ProgressBar::value() const
{
    return _value;
}

int ProgressBar::minimum() const
{
    return _min;
}

int ProgressBar::maximum() const
{
    return _max;
}
