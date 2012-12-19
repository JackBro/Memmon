#include "datachart.h"
#include "../mmutil.h"

#include <QVBoxLayout>
#include <QDesktopWidget>

/*******************************************/
/*! DataChart                              */
/*******************************************/
DataChart* DataChart::_instance = NULL;
DataChart::DataChart(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout* topLayout = new QHBoxLayout;

    QSize buttonSize(NS_DataChart::Constant::CaptionHeight,NS_DataChart::Constant::CaptionHeight);
    _pinButton = new QPushButton(this);
    _pinButton->setIcon(QIcon(":/images/pin.png"));
    _pinButton->setFixedSize(buttonSize);
    _pinButton->setCheckable(true);
    _pinButton->setAutoRepeat(true);
    _pinButton->setStyleSheet("background-color:rgb(153,153,153);");
    connect(_pinButton,SIGNAL(clicked()),this,SLOT(slot_pinThis()));

    _closeButton = new QPushButton(this);
    _closeButton->setIcon(QIcon(":/images/close.png"));
    _closeButton->setFixedSize(buttonSize);
    _closeButton->setAutoRepeat(true);
    _closeButton->setStyleSheet("background-color:rgb(153,153,153);");
    connect(_closeButton,SIGNAL(clicked()),this,SLOT(close()));

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    _d = new DataChartPrivate(this);
    _caption = new DataChartCaption(this);

    topLayout->addWidget(_pinButton);
    topLayout->addWidget(_caption);
    topLayout->addWidget(_closeButton);

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(_d);
    setLayout(mainLayout);

    setWindowFlags(Qt::ToolTip);
    setThisSize();

    initTimer();
}

DataChart* DataChart::instance()
{
    if(_instance == NULL)
    {
        _instance = new DataChart;
    }

    return _instance;
}

void DataChart::setData(const QList<qreal> &data)
{
    resetTimer();
    _d->setData(data);
}

void DataChart::clear()
{
    _d->clear();
}

void DataChart::showAt(const QPoint &showPoint)
{
    QDesktopWidget desktop;
    QRect deskRect = desktop.geometry();

    qreal topLeftX = showPoint.x();
    qreal topLeftY = showPoint.y();

    if((topLeftX + width())> deskRect.width())
    {
        topLeftX = deskRect.width() - width() - NS_DataChart::Constant::ExtraSpace;
    }

    if((topLeftY + height()) > deskRect.height())
    {
        topLeftY = deskRect.height() - height() - NS_DataChart::Constant::ExtraSpace;
    }

    QPoint newMovePoint(topLeftX,topLeftY);
    move(newMovePoint);
    show();
}

void DataChart::setCaption(const QString &strCaption)
{
    _caption->setCaption(strCaption);
}

QString DataChart::caption() const
{
    return _caption->caption();
}

void DataChart::setThisSize()
{
    QDesktopWidget desktop;
    qreal thisWidth = desktop.geometry().width()/3;
    qreal thisHeight = thisWidth * 0.68;
    setFixedSize(thisWidth,thisHeight);
}

void DataChart::initTimer()
{
    _hideTimer = new QTimer(this);
    _hideTimer->setInterval(NS_DataChart::Constant::StayDuration);
    connect(_hideTimer,SIGNAL(timeout()),this,SLOT(hide()));

}

void DataChart::resetTimer()
{
    if(_hideTimer->isActive())
    {
        _hideTimer->stop();
        _hideTimer->start();
    }
}

void DataChart::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);
    _hideTimer->start();
}

void DataChart::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);
    _pressed = true;
    _offset = e->pos();
}

void DataChart::mouseMoveEvent(QMouseEvent *e)
{
    if(_pressed)
    {
        move(e->globalPos() - _offset);
    }
}

void DataChart::slot_pinThis()
{
    if(_pinButton->isChecked())
    {
        if(_hideTimer->isActive())
        {
            _hideTimer->stop();
        }
    }
    else
    {
        if(!_hideTimer->isActive())
        {
            _hideTimer->start();
        }
    }
}

/*******************************************/
/*! DataChartPrivate                       */
/*******************************************/
DataChartPrivate::DataChartPrivate(QWidget *parent):QWidget(parent)
{
    _maxValue = 0;
}

void DataChartPrivate::setData(const QList<qreal> &data)
{
    _data = data;
    findMaxValue();
    update();
}

void DataChartPrivate::clear()
{
    _data.clear();
    _maxValue = 0;
    update();
}

void DataChartPrivate::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    drawBackground(&painter);
    drawLegend(&painter);
    drawGraph(&painter);
}

void DataChartPrivate::drawBackground(QPainter *painter)
{
    painter->save();

    QLinearGradient bgGradient(rect().topLeft(),rect().bottomLeft());
    bgGradient.setColorAt(0.0,NS_DataChart::Color::ChartStartColor);
    bgGradient.setColorAt(1.0,NS_DataChart::Color::ChartStopColor);
    painter->setBrush(bgGradient);
    painter->setPen(Qt::NoPen);

    painter->drawRect(rect());

    painter->restore();
}

void DataChartPrivate::drawLegend(QPainter *painter)
{
    painter->save();
    painter->setPen(NS_DataChart::Color::LegendColor);
    // draw vertical legend
    qreal maxTextLength = fontMetrics().width(tr("%1").arg(_maxValue));
    qreal maxLeftSpace = NS_DataChart::Constant::ExtraSpace + maxTextLength;
    _maxLeftSpace = maxLeftSpace;
    qreal initTextRight = maxLeftSpace - NS_DataChart::Constant::ExtraSpace;
    qreal dY = (qreal)(height() - 2 * NS_DataChart::Constant::BottomSpace)/2;
    qreal initY = NS_DataChart::Constant::BottomSpace;
    qreal dValue = (qreal)_maxValue/NS_DataChart::Constant::VerticalGridCount;
    qreal initValue = _maxValue;
    qreal textLength = 0;
    QString strValue;

    for(int i = 0;i <= NS_DataChart::Constant::VerticalGridCount;i++)
    {
        strValue = tr("%1").arg(MM_Util::getDecentSize(initValue));
        textLength = fontMetrics().width(strValue);
        QPointF textPoint(initTextRight - textLength,initY + fontMetrics().height()/2);
        painter->drawText(textPoint,strValue);

        // increment initY
        initY += dY;

        // decrement value
        initValue -= dValue;
    }

    QPoint topPoint(maxLeftSpace,NS_DataChart::Constant::BottomSpace);
    QPoint bottomPoint(topPoint.x(),height() - NS_DataChart::Constant::BottomSpace);
    painter->drawLine(topPoint,bottomPoint);

    drawLittleAngle(painter,topPoint,Qt::Vertical);

    // draw horizontal legend
    QPoint leftPoint(maxLeftSpace,height() - NS_DataChart::Constant::BottomSpace);
    QPoint rightPoint(width() - NS_DataChart::Constant::LeftSpace,leftPoint.y());
    painter->drawLine(leftPoint,rightPoint);

    drawLittleAngle(painter,rightPoint,Qt::Horizontal);

    QFont textFont;
    textFont.setBold(true);
    painter->setFont(textFont);

    QPoint topLeft(maxLeftSpace,height() - NS_DataChart::Constant::BottomSpace);
    QPoint bottomRight(width() - NS_DataChart::Constant::LeftSpace,height());
    QRect textRect(topLeft,bottomRight);
    painter->drawText(textRect,tr("Recent Data Set"),Qt::AlignVCenter|Qt::AlignRight);


    painter->restore();
}

void DataChartPrivate::drawGraph(QPainter *painter)
{
    painter->save();
    painter->setPen(NS_DataChart::Color::GraphColor);

    qreal validGraphHeight = height() - 2 * NS_DataChart::Constant::BottomSpace;
    qreal validGraphWidth = width() - _maxLeftSpace - NS_DataChart::Constant::LeftSpace;

    qreal initX = _maxLeftSpace;
    qreal dX = (qreal)validGraphWidth/_data.size();

    bool hasEnoughSpaceForDrawingBox = true;
    qreal drawBoxNeededSpace = _data.size() * NS_DataChart::Constant::AngleEdgeLength +
                               (_data.size() - 1) * NS_DataChart::Constant::ExtraSpace;
    if(validGraphWidth < drawBoxNeededSpace)
    {
        hasEnoughSpaceForDrawingBox = false;
    }

    QVector<QPointF> pots;
    for(int i = 0;i < _data.size();i++)
    {
        qreal pointY = height() - NS_DataChart::Constant::BottomSpace - (_data.at(i) * validGraphHeight)/_maxValue;
        QPointF center(initX,pointY);
        pots.push_back(center);

        if(hasEnoughSpaceForDrawingBox)
        {
            drawRotatedLittleRect(painter,center.toPoint());
        }
        // increment x
        initX += dX;
    }

    // draw polylines
    QVector<QPointF> graphPots;
    for(int i = 0;i < pots.size();i++)
    {
        if(i > 0 && i < (pots.size() - 1))
        {
            graphPots.push_back(pots.at(i));
            graphPots.push_back(pots.at(i));
        }
        else
        {
            graphPots.push_back(pots.at(i));
        }
    }

    painter->drawPolyline(QPolygonF(graphPots));





    painter->restore();
}

void DataChartPrivate::drawLittleAngle(QPainter* painter,const QPoint& vertex,Qt::Orientation o)
{
    painter->save();
    painter->setPen(NS_DataChart::Color::LegendColor);
//    painter->setRenderHints(QPainter::Antialiasing);

    if(o == Qt::Vertical)
    {
        QPoint bottomLeft(vertex.x() - NS_DataChart::Constant::AngleEdgeLength,vertex.y() + NS_DataChart::Constant::AngleEdgeLength);
        painter->drawLine(vertex,bottomLeft);

        QPoint bottomRight(vertex.x() + NS_DataChart::Constant::AngleEdgeLength,vertex.y() + NS_DataChart::Constant::AngleEdgeLength);
        painter->drawLine(vertex,bottomRight);
    }
    else
    {
        QPoint topLeft(vertex.x() - NS_DataChart::Constant::AngleEdgeLength,vertex.y() - NS_DataChart::Constant::AngleEdgeLength);
        painter->drawLine(vertex,topLeft);

        QPoint bottomLeft(vertex.x() - NS_DataChart::Constant::AngleEdgeLength,vertex.y() + NS_DataChart::Constant::AngleEdgeLength);
        painter->drawLine(vertex,bottomLeft);
    }
    painter->restore();
}

void DataChartPrivate::drawRotatedLittleRect(QPainter* painter,const QPoint& center)
{
    painter->save();

    painter->translate(center);
    painter->save();
    painter->rotate(45);

    QPoint topLeft(0 - NS_DataChart::Constant::LittleRectWidth,0 - NS_DataChart::Constant::LittleRectWidth);
    QPoint bottomRight(0 + NS_DataChart::Constant::LittleRectWidth,0 + NS_DataChart::Constant::LittleRectWidth);

    painter->setPen(Qt::NoPen);
    painter->setBrush(NS_DataChart::Color::LittleRectColor);
    painter->drawRect(QRect(topLeft,bottomRight));
    painter->restore();

    painter->restore();
}

void DataChartPrivate::findMaxValue()
{
    qreal maxValue = 0;
    for(int i = 0;i < _data.size();i++)
    {
        if(maxValue < _data.at(i))
        {
            maxValue = _data.at(i);
        }
    }

    _maxValue = maxValue * 1.5;
}


/*******************************************/
/*! DataChartCaption                       */
/*******************************************/
DataChartCaption::DataChartCaption(QWidget *parent):QWidget(parent)
{
    setFixedHeight(NS_DataChart::Constant::CaptionHeight);
}


void DataChartCaption::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    draw(&painter);
}

void DataChartCaption::draw(QPainter *painter)
{
    painter->save();

    painter->setPen(Qt::NoPen);
    painter->setBrush(NS_DataChart::Color::BackgroundColor);
    painter->drawRect(rect());

    painter->setPen(NS_DataChart::Color::CaptionColor);

    QFont textFont;
    textFont.setBold(true);
    painter->setFont(textFont);

    painter->drawText(rect(),_strCaption,Qt::AlignVCenter|Qt::AlignHCenter);
    painter->restore();
}

void DataChartCaption::setCaption(const QString &strCaption)
{
    _strCaption = strCaption;
    update();
}

QString DataChartCaption::caption() const
{
    return _strCaption;
}
