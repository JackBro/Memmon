#include "memchart.h"
#include "memchart_p.h"
#include <QVBoxLayout>
#include <QToolTip>
#include "logutil.h"
#include "pyhistogramtip.h"
#include <QMessageBox>
#include "mmutil.h"

/**************************************************/
/*!        MemChartPrivate                        */
/**************************************************/
MemChartPrivate::MemChartPrivate(QWidget *parent):QWidget(parent)
{
    initVars();
    initSettings();
    initContextMenu();
}

///
/// private utility functions
///
void MemChartPrivate::initVars()
{
    _isCurveVisible = true;
    _isHistogramVisible = true;
    _mousePressed = false;
    _isAcceptData = true;

    _min = 0;
    _max = 0;
    _maxValue = 0;
    _histogramWidth = MC::Constant::MinHistogramWidth;
    _alignment = Qt::AlignCenter;
}

void MemChartPrivate::initSettings()
{
    setMouseTracking(true);
}

void MemChartPrivate::showHighlightTip()
{
    qreal selectedHeight = qAbs(_pressedPoint.y() - _movePoint.y());
    int selectedSize = selectedHeight * (_max - _min) / height();
    QString strSize = MM_Util::getDecentSize(selectedSize);
    QToolTip::showText(mapToGlobal(_movePoint.toPoint()),strSize);
//    showFancyTip(mapToGlobal(_movePoint.toPoint()),strSize);
}

QPointF MemChartPrivate::constructPoint(QMouseEvent *e)
{
    qreal x = e->pos().x();
    qreal y = 0;

    if(e->pos().y() < MC::Constant::TopSpace)
    {
        y = MC::Constant::TopSpace + MC::Constant::FrameWidth;
    }
    else if(e->pos().y() > (height() - MC::Constant::TopSpace))
    {
        y = height() - MC::Constant::TopSpace - MC::Constant::FrameWidth;
    }
    else
    {
        y = e->pos().y();
    }

    QPointF constrPoint(x,y);
    return constrPoint;
}

void MemChartPrivate::adjustElementCount()
{
    int eleSize = _values.size();
    qreal eleWidth = MC::Constant::MinHistogramWidth * eleSize ;
    int spaceSize = eleSize + 1;
    qreal spaceWidth = spaceSize * MC::Constant::MinHistogramSpace;

    if((eleWidth + spaceWidth)> width())
    {
        _values.pop_front();
    }
}

void MemChartPrivate::initContextMenu()
{
    _contextMenu = new QMenu(tr("Context Menu"),this);

    _showCurveAct = new QAction(tr("Show Curve"),this);
    _showCurveAct->setCheckable(true);
    _showCurveAct->setChecked(true);

    _showHistogramAct = new QAction(tr("Show Histogram"),this);
    _showHistogramAct->setCheckable(true);
    _showHistogramAct->setChecked(true);

    _acceptDataAct = new QAction(tr("Accept Data"),this);


    connect(_showCurveAct,SIGNAL(triggered()),this,SLOT(ActionHandler()));
    connect(_showHistogramAct,SIGNAL(triggered()),this,SLOT(ActionHandler()));
    connect(_acceptDataAct,SIGNAL(triggered()),this,SLOT(ActionHandler()));

    _contextMenu->addAction(_showCurveAct);
    _contextMenu->addAction(_showHistogramAct);
    _contextMenu->addAction(_acceptDataAct);


}


void MemChartPrivate::showFancyTip(const QPointF& globalPoint,const QString &strTip)
{
    PYHistogramTip* tip = PYHistogramTip::getInstance();
    tip->raise();
    tip->setInterval(3);
    tip->setDirection(true);
    tip->setTip(strTip);
    tip->move(globalPoint.toPoint());
    tip->show();
}

void MemChartPrivate::resetPoints()
{
    _pressedPoint = QPointF();
    _movePoint = QPointF();
}

///
/// private slot functions
///
void MemChartPrivate::ActionHandler()
{
    QAction* who = qobject_cast<QAction*>(sender());

    if(who == _showCurveAct)
    {
        _isCurveVisible = !_isCurveVisible;
    }

    if(who == _showHistogramAct)
    {
        _isHistogramVisible = !_isHistogramVisible;
    }

    if(who == _acceptDataAct)
    {
        _isAcceptData = !_isAcceptData;
        _acceptDataAct->setText(_isAcceptData ? tr("Unaccept Data") : tr("Accept Data"));
    }

    update();
}

///
/// reimplemented functions
///
void MemChartPrivate::paintEvent(QPaintEvent *e)
{

    QPainter painter(this);
    drawBackground(&painter);
    drawCaption(&painter);
    drawFrame(&painter);
    drawLegend(&painter);
    drawHistogram(&painter);
    drawSelectedRegion(&painter);
}

void MemChartPrivate::mouseMoveEvent(QMouseEvent *e)
{
    if(_mousePressed)
    {
        _movePoint = constructPoint(e);
        update();
        showHighlightTip();
    }

    for(int barIndex = 0;barIndex < _barItems.size();barIndex++)
    {
        if(_barItems.at(barIndex).first.contains(e->pos()))
        {
//            QToolTip::showText(e->globalPos(),_barItems.at(barIndex).second);
            QPointF barTopPoint(_barItems.at(barIndex).first.center().x(),_barItems.at(barIndex).first.topLeft().y());
            QPointF gBarTopPoint = mapToGlobal(barTopPoint.toPoint());
            showFancyTip(gBarTopPoint,_barItems.at(barIndex).second);
            break;
        }
    }
}

void MemChartPrivate::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)
    {
        _mousePressed = true;
        _pressedPoint = constructPoint(e);
    }
}

void MemChartPrivate::mouseReleaseEvent(QMouseEvent *e)
{
    _mousePressed = false;
    resetPoints();
    update();
}

void MemChartPrivate::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
    adjustElementCount();
}

void MemChartPrivate::contextMenuEvent(QContextMenuEvent *e)
{
    _contextMenu->exec(e->globalPos());
    e->accept();
}

void MemChartPrivate::leaveEvent(QEvent *e)
{
    QWidget::leaveEvent(e);
    resetPoints();
    PYHistogramTip* tip = PYHistogramTip::getInstance();
    tip->hide();
}

///
/// painting functions
///
void MemChartPrivate::drawBackground(QPainter* painter)
{
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);
    drawRect(painter,rect(),Qt::Vertical,MC::Color::StartBackground,MC::Color::StopBackground);
    painter->restore();
}

void MemChartPrivate::drawCaption(QPainter *painter)
{
    painter->save();
    painter->setPen(MC::Color::CaptionColor);

    QFont captionFont;
    captionFont.setBold(true);
    painter->setFont(captionFont);

    qreal maxTextLength = fontMetrics().width(tr("%1").arg(_max));
    qreal captionHeight = fontMetrics().height() + 2 * MC::Constant::ExtraSpace;
    qreal topX = 3 * MC::Constant::ExtraSpace + maxTextLength;
    qreal topY = MC::Constant::TopSpace + MC::Constant::ExtraSpace;

    QRectF captionRect(QPointF(topX,topY),QPointF(width() - topX,topY + captionHeight));

    painter->drawText(captionRect,_caption,_alignment);



    painter->restore();
}

void MemChartPrivate::drawFrame(QPainter *painter)
{
    qreal textLength = fontMetrics().width(tr("%1").arg(_max));
    qreal hSpace = 2 * MC::Constant::ExtraSpace + textLength;

    painter->save();

    QPointF topLeft(hSpace,MC::Constant::TopSpace);
    QPointF bottomRight(width() - hSpace,height() - MC::Constant::TopSpace);
    painter->setPen(MC::Color::LegendColor);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(QRectF(topLeft,bottomRight));


    painter->restore();
}

void MemChartPrivate::drawLegend(QPainter* painter)
{
    qreal maxTextLength = fontMetrics().width(tr("%1").arg(_max));

    painter->save();
    painter->setPen(MC::Color::TextColor);

    qreal initY = MC::Constant::TopSpace;
    qreal dY = (qreal)(height() - 2 * MC::Constant::TopSpace)/MC::Constant::GridCount;

    qreal dValue = (qreal)(_max - _min)/MC::Constant::GridCount;
    int initValue = _max;
    QString strText;

    // draw left legend
//    drawLegendText(painter,MC::Constant::ExtraSpace + maxTextLength - textLength);
    for(int i = 0;i <= MC::Constant::GridCount;i++)
    {
        // draw text
        strText = tr("%1").arg(initValue);
        qreal textLength = fontMetrics().width(strText);
        QPointF textPoint(MC::Constant::ExtraSpace + maxTextLength - textLength ,initY + fontMetrics().height()/2);
        painter->drawText(textPoint,strText);

        // draw mark
        QPointF handleLeftPoint(MC::Constant::ExtraSpace + maxTextLength,initY);
        QPointF handleRightPoint(handleLeftPoint.x() + MC::Constant::ExtraSpace,initY);
        painter->drawLine(handleLeftPoint,handleRightPoint);

        // increment initY
        initY += dY;

        // decrement value
        initValue -= dValue;


        //
        initValue = initValue < 0 ? 0 : initValue;
    }

    initY = MC::Constant::TopSpace;

    // draw right legend
    initValue = _max;
    for(int i = 0;i <= MC::Constant::GridCount;i++)
    {
        // draw text
        strText = tr("%1").arg(initValue);
        qreal textLength = fontMetrics().width(strText);
        QPointF textPoint(width() - MC::Constant::ExtraSpace - maxTextLength,initY + fontMetrics().height()/2);
        painter->drawText(textPoint,strText);

        // draw mark
        QPointF handleLeftPoint(width() - MC::Constant::ExtraSpace * 2 - maxTextLength,initY);
        QPointF handleRightPoint(handleLeftPoint.x() + MC::Constant::ExtraSpace,initY);
        painter->drawLine(handleLeftPoint,handleRightPoint);

        // increment initY
        initY += dY;

        // decrement value
        initValue -= dValue;

        //
        initValue = initValue < 0 ? 0 : initValue;
    }

    painter->restore();
}

void MemChartPrivate::drawLegendText(QPainter *painter,qreal initX)
{
    painter->save();
    qreal initY = MC::Constant::TopSpace;
    qreal dY = (qreal)(height() - 2 * MC::Constant::TopSpace)/MC::Constant::GridCount;

    qreal maxTextLength = fontMetrics().width(tr("%1").arg(_max));
    qreal dValue = (qreal)(_max - _min)/MC::Constant::GridCount;
    int initValue = _max;
    QString strText;

    for(int i = 0;i <= MC::Constant::GridCount;i++)
    {
        // draw text
        strText = tr("%1").arg(initValue);
        qreal textLength = fontMetrics().width(strText);
        QPointF textPoint(initX ,initY + fontMetrics().height()/2);
        painter->drawText(textPoint,strText);
        // increment initY
        initY += dY;

        // decrement value
        initValue -= dValue;

        //
        initValue = initValue < 0 ? 0 : initValue;
    }

    painter->restore();

}

void MemChartPrivate::drawHistogram(QPainter* painter)
{

    _barItems.clear();

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing|QPainter::HighQualityAntialiasing);
    painter->setPen(MC::Color::HistogramPen);

    qreal maxTextLength = fontMetrics().width(tr("%1").arg(_max));
    qreal occupiedSpace = maxTextLength + 2 * MC::Constant::ExtraSpace;
    int eleCount = _values.size();
    qreal dSpace = (width() - occupiedSpace * 2)/(eleCount + 1);

    if(dSpace < MC::Constant::MinHistogramSpace)
    {
        dSpace = MC::Constant::MinHistogramSpace;
    }

    qreal initX = occupiedSpace + dSpace;
    qreal graphHeight = height() - 2 * MC::Constant::TopSpace;

    // points used to draw curve
    QVector<QPointF> topPoints;
    painter->save();
    painter->setOpacity(0.8);
    for(int eleIndex = 0;eleIndex < eleCount;eleIndex++)
    {
        qreal histogramHeight = (qreal)_values.at(eleIndex) * graphHeight / (_max - _min);

        QPointF topLeft(initX - _histogramWidth/2, height() - MC::Constant::TopSpace - histogramHeight);
        QPointF bottomRight(initX + _histogramWidth/2,height() - MC::Constant::TopSpace - 1);
        QRectF barRect(topLeft,bottomRight);

        if(_isHistogramVisible)
        {
            drawRect(painter,barRect,Qt::Horizontal,MC::Color::HistogramStart,MC::Color::HistogramStop);
        }

        BarItem item(barRect,MM_Util::getDecentSize(_values.at(eleIndex)));
        _barItems.push_back(item);

        initX += dSpace;

        topPoints.push_back(QPointF(barRect.center().x(),topLeft.y()));

    }

    painter->restore();


    // put drawCurve code in drawHistogram so we don't need to calculate the x/ys once again
    if(_isCurveVisible)
    {
        QVector<QPointF> curvePoints;
        for(int i = 0;i < topPoints.size();i++)
        {
            if(i > 0 && i < (topPoints.size() -1))
            {
                curvePoints.push_back(topPoints.at(i - 1));
            }

            curvePoints.push_back(topPoints.at(i));
        }

        painter->setPen(QPen(MC::Color::CurveColor,MC::Constant::CurvePenWidth));
        painter->drawPolyline(QPolygonF(curvePoints));
    }




    painter->restore();
}

void MemChartPrivate::drawSelectedRegion(QPainter* painter)
{
    if(!_mousePressed || _pressedPoint.isNull() ||  _movePoint.isNull())
    {
        return;
    }
    painter->save();

    qreal maxTextLength = fontMetrics().width(tr("%1").arg(_max));
    qreal hSpace = maxTextLength + 2 * MC::Constant::ExtraSpace;
    painter->setPen(MC::Color::HighlightColor);
    painter->setBrush(MC::Color::HighlightColor);
    painter->setOpacity(0.5);

    QRectF highlightRect(QPointF(hSpace + MC::Constant::FrameWidth,_pressedPoint.y()),QPointF(width() - hSpace - MC::Constant::FrameWidth,_movePoint.y()));
    painter->drawRect(highlightRect);


    painter->restore();
}

QBrush MemChartPrivate::getBrush(const QRectF& rect,Qt::Orientation o,const QColor& startColor,const QColor& stopColor)
{
    switch(o)
    {
        case Qt::Horizontal:
        {
            QLinearGradient hGradient(rect.topLeft(),rect.topRight());
            hGradient.setColorAt(0.0,startColor);
            hGradient.setColorAt(1.0,stopColor);
            return hGradient;
        }
        case Qt::Vertical:
        {
            QLinearGradient vGradient(rect.topLeft(),rect.bottomLeft());
            vGradient.setColorAt(0.0,startColor);
            vGradient.setColorAt(1.0,stopColor);
            return vGradient;
        }
    }
}

void MemChartPrivate::drawRect(QPainter *painter, const QRectF &rect, Qt::Orientation o, const QColor &startColor, const QColor &stopColor)
{
    painter->setBrush(getBrush(rect,o,startColor,stopColor));
    painter->drawRect(rect);
}

///
/// public functions
///
void MemChartPrivate::setCaption(const QString &strCaption)
{
    _caption = strCaption;
}

QString MemChartPrivate::caption() const
{
    return _caption;
}

void MemChartPrivate::setCaptionAlignment(Qt::Alignment alignment)
{
    _alignment = alignment;
}

Qt::Alignment MemChartPrivate::captionAlignment() const
{
    return _alignment;
}

void MemChartPrivate::addValue(int value)
{
    if(!_isAcceptData)
    {
        return;
    }

    if(_maxValue < value)
    {
        _maxValue = value;
        _max = _maxValue * MC::Constant::MaxRatio;
    }

    _values.push_back(value);

    adjustElementCount();
    update();

}

void MemChartPrivate::setCurveVisible(bool visible)
{
    _isCurveVisible = visible;
    update();
}

bool MemChartPrivate::isCurveVisible() const
{
    return _isCurveVisible;
}

void MemChartPrivate::setHistogramVisible(bool visible)
{
    _isHistogramVisible = visible;
    update();
}

bool MemChartPrivate::isHistogramVisible() const
{
    return _isHistogramVisible;
}

void MemChartPrivate::clear()
{
    _values.clear();
    _max = 0;
    _maxValue = 0;
    _min = 0;
    update();
}

/**************************************************/
/*!        MemChart                               */
/**************************************************/
MemChart::MemChart(QWidget *parent) :
    QWidget(parent)
{
    d_ptr = new MemChartPrivate(this);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(d_ptr);

    setLayout(mainLayout);
}

///
/// public functions
///
void MemChart::setCaption(const QString& strCaption)
{
    Q_D(MemChart);
    d->setCaption(strCaption);
}

QString MemChart::caption() const
{
    Q_D(const MemChart);
    return d->caption();
}

void MemChart::setCaptionAlignment(Qt::Alignment alignment)
{
    Q_D(MemChart);
    d->setCaptionAlignment(alignment);
}

Qt::Alignment MemChart::captionAlignment() const
{
    Q_D(const MemChart);
    return d->captionAlignment();
}

void MemChart::addValue(int value)
{
    Q_D(MemChart);
    d->addValue(value);
}

void MemChart::setCurveVisible(bool visible)
{
    Q_D(MemChart);
    d->setCurveVisible(visible);
}

bool MemChart::isCurveVisible() const
{
    Q_D(const MemChart);
    return d->isCurveVisible();
}

void MemChart::setHistogramVisible(bool visible)
{
    Q_D(MemChart);
    d->setHistogramVisible(visible);
}

bool MemChart::isHistogramVisible() const
{
    Q_D(const MemChart);
    return d->isHistogramVisible();
}

void MemChart::clear()
{
    Q_D(MemChart);
    d->clear();
}
