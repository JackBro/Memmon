#include "pyprog.h"

#define PYP_LEFT_SPACE 10
#define PYP_TOP_SPACE 2
#define PYP_RIGHT_SPACE 7
#define PYP_TEXT_WIDTH 45
#define PYP_RECT_RADIUS 3
#define PYP_LINE_COUNT 20
#define PYP_LINE_WIDTH 2
#define PYP_D_LINE 7
#define PYP_UPDATE_STEP 10
#define PYP_UPDATE_INTERVAL 10

#define PYP_LINE_COLOR Qt::black
#define PYP_BG_COLOR QColor(45,45,45)

#define PYP_BAR_START_COLOR QColor(85,255,0)
#define PYP_BAR_MIDDLE_COLOR QColor(249,216,47)
#define PYP_BAR_END_COLOR QColor(249,107,24)
#define PYP_UNDER_BAR_COLOR QColor(121,121,121)

#define PYP_TEXT_COLOR Qt::white

PYProg::PYProg(QWidget *parent) :
    QWidget(parent)
{
    initVariables();
}

/*!
  private utility functions
*/
void PYProg::initVariables()
{
    m_min = 0;
    m_max = 100;
    m_value = 0;
    m_currValue = 0;
}

void PYProg::initAnimationTimer()
{
    m_animationTimer = new QTimer(this);
    m_animationTimer->setInterval(PYP_UPDATE_INTERVAL);
    connect(m_animationTimer,SIGNAL(timeout()),this,SLOT(slot_updateValue()));
}

/*!
 * private slot functions
 */
void PYProg::slot_updateValue()
{
    if(m_isIncreValue)
    {
        m_currValue += m_dValue;

        if(m_currValue >= m_value)
        {
            m_animationTimer->stop();
        }
    }
    else
    {
        m_currValue -= m_dValue;

        if(m_currValue <= m_value)
        {
            m_animationTimer->stop();
        }
    }
    update();
}

/*!
 * reimpl
 */
void PYProg::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing|QPainter::HighQualityAntialiasing);
    drawBg(&painter);
    drawBar(&painter);
}

void PYProg::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);
    emit sig_mousePressed();
}

/*!
  painting functions
*/
void PYProg::drawBg(QPainter *painter)
{
    painter->save();
    painter->setPen(PYP_BG_COLOR);

    QLinearGradient BgGradient(QPoint(0,0),QPoint(width(),0));
    BgGradient.setColorAt(0.0,QColor(222,222,222));
    BgGradient.setColorAt(0.05,PYP_BG_COLOR);
    BgGradient.setColorAt(1.0,PYP_BG_COLOR);
    painter->setBrush(BgGradient);
    painter->drawRoundedRect(rect(),PYP_RECT_RADIUS,PYP_RECT_RADIUS);

    painter->restore();
}

void PYProg::drawBar(QPainter *painter)
{
    painter->save();

    // DRAW UNDER BAR
    painter->setPen(Qt::NoPen);

    QPointF uTopLeft(PYP_LEFT_SPACE,PYP_TOP_SPACE);
    QPointF uBottomRight(width() - PYP_RIGHT_SPACE * 2 - PYP_TEXT_WIDTH,height() - PYP_TOP_SPACE);
    QRectF uRect(uTopLeft,uBottomRight);
    painter->setBrush(PYP_UNDER_BAR_COLOR);
    painter->drawRoundedRect(uRect,PYP_RECT_RADIUS,PYP_RECT_RADIUS);

    //  DRAW REAL BAR
    qreal range = (m_max - m_min);
    qreal RectWidth = width() - PYP_LEFT_SPACE  - PYP_RIGHT_SPACE * 2 - PYP_TEXT_WIDTH;
    qreal dX = (qreal) RectWidth/range;

    qreal initX = PYP_LEFT_SPACE;
    QPointF TopLeft(initX,PYP_TOP_SPACE);
    QPointF BottomRight(initX + dX * m_currValue,height() - PYP_TOP_SPACE);
    QRectF BarRect(TopLeft,BottomRight);

    QLinearGradient BarGradient(uRect.topLeft(),uRect.topRight());
    BarGradient.setColorAt(0.0,PYP_BAR_START_COLOR);
    BarGradient.setColorAt(0.5,PYP_BAR_MIDDLE_COLOR);
    BarGradient.setColorAt(1.0,PYP_BAR_END_COLOR);

    painter->setBrush(BarGradient);

    painter->drawRoundedRect(BarRect,PYP_RECT_RADIUS,PYP_RECT_RADIUS);

    // DRAW COVER LINES

    int nLineCnt = uRect.width()/PYP_D_LINE;
    int dLineX = (qreal)uRect.width()/(nLineCnt - 1);
    painter->setPen(PYP_LINE_COLOR);
    while(initX < uRect.width())
    {
        QPointF topPot(initX + dLineX,PYP_TOP_SPACE);
        QPointF bottomPot(initX + dLineX,height() - PYP_TOP_SPACE);
        painter->drawLine(topPot,bottomPot);

        /// increment initX
        initX += dLineX;
    }

    // draw text
    QPointF TextTopLeft(width() - PYP_RIGHT_SPACE - PYP_TEXT_WIDTH,PYP_TOP_SPACE);
    QPointF TextBottomRight(width() - PYP_RIGHT_SPACE,height() - PYP_TOP_SPACE);
    QRectF TextRect(TextTopLeft,TextBottomRight);
    painter->setPen(PYP_UNDER_BAR_COLOR);
    painter->setBrush(PYP_UNDER_BAR_COLOR);
    painter->drawRoundedRect(TextRect,PYP_RECT_RADIUS,PYP_RECT_RADIUS);

    painter->setPen(PYP_TEXT_COLOR);

    QFont TextFont("Times New Roman");
    TextFont.setBold(true);
    painter->setFont(TextFont);

    QString strText;
    if(m_strText.isEmpty())
    {
        qreal value = (qreal)m_value * 100/(m_max - m_min);
        strText = tr("%1%").arg(value,3,'f',1,'0');
    }
    else
    {
        strText = m_strText;
    }
    painter->drawText(TextRect,strText,Qt::AlignVCenter|Qt::AlignHCenter);
    painter->restore();
}

/*!
 * public interfaces
 */
void PYProg::setRange(qreal min, qreal max)
{
    Q_ASSERT(min < max);
    m_min = min;
    m_max = max;
    update();
}

void PYProg::setValue(qreal value)
{
    if(!m_isAnimated)
    {
        m_value = value;
        m_currValue = value;
        update();
    }
    else
    {
        if(value != m_value)
        {
            m_dValue = (qreal)qAbs(value - m_value)/PYP_UPDATE_STEP;
            m_animationTimer->start();
            m_isIncreValue = value > m_value;
            m_value = value;
        }
    }
}

void PYProg::setText(const QString &strText)
{
    m_strText = strText;
}

QString PYProg::text() const
{
    return m_strText;
}

void PYProg::setAnimated(bool animated)
{
    m_isAnimated = animated;
    if(m_isAnimated)
    {
        initAnimationTimer();
    }
}

bool PYProg::isAnimated() const
{
    return m_isAnimated;
}
