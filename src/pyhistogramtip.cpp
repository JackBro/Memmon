#include "pyhistogramtip.h"

PYHistogramTip* PYHistogramTip::mInstance = NULL;
PYHistogramTip::PYHistogramTip(QWidget *parent) :
    QWidget(parent)
{
    initVariables();
    initSettings();
}

/*!
  protected functions
*/
void PYHistogramTip::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    draw(&painter);
}

/*!
  private utility functions
*/
void PYHistogramTip::initVariables()
{

    m_width = sizeHint().width();
    m_height = sizeHint().height();

    m_bLeft = true;

    m_nInterval = 3;
    m_hideTimer=new QTimer(this);
    m_hideTimer->setInterval(m_nInterval * 1000);
    connect(m_hideTimer,SIGNAL(timeout()),this,SLOT(close()));
}

void PYHistogramTip::initSettings()
{
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);
}

/*!
  painting functions
*/
void PYHistogramTip::draw(QPainter *painter)
{
    painter->save();

    QPainterPath BgPath;

    if(m_bLeft)
    {
        QPointF LeftPot(0,PYHT_BG_TOP_SPACE*2);
        QPointF LeftTopPot(PYHT_BG_LEFT_SPACE,PYHT_BG_TOP_SPACE);
        QPointF TopLeftPot(PYHT_BG_LEFT_SPACE,0);
        QPointF RightTopPot(width(),0);
        QPointF RightBottomPot(width(),height());
        QPointF LeftBottomPot(PYHT_BG_LEFT_SPACE,height());
        QPointF LeftBottomTopPot(PYHT_BG_LEFT_SPACE,PYHT_BG_TOP_SPACE*2);

        BgPath.moveTo(LeftPot);
        BgPath.lineTo(LeftTopPot);
        BgPath.lineTo(TopLeftPot);
        BgPath.lineTo(RightTopPot);
        BgPath.lineTo(RightBottomPot);
        BgPath.lineTo(LeftBottomPot);
        BgPath.lineTo(LeftBottomTopPot);
        BgPath.lineTo(LeftPot);

    }else{
        QPointF TopLeftPot(0,0);
        QPointF TopRightPot(width() - PYHT_BG_LEFT_SPACE,0);
        QPointF RightTopPot(width() - PYHT_BG_LEFT_SPACE,PYHT_BG_TOP_SPACE);
        QPointF RightPot(width(),height()/2);
        QPointF RightBottomPot(width() - PYHT_BG_LEFT_SPACE,height()/2);
        QPointF BottomRightPot(width()-PYHT_BG_LEFT_SPACE,height());
        QPointF BottomLeftPot(0,height());

        BgPath.moveTo(TopLeftPot);
        BgPath.lineTo(TopRightPot);
        BgPath.lineTo(RightTopPot);
        BgPath.lineTo(RightPot);
        BgPath.lineTo(RightBottomPot);
        BgPath.lineTo(BottomRightPot);
        BgPath.lineTo(BottomLeftPot);
        BgPath.lineTo(TopLeftPot);

    }

    painter->setPen(Qt::NoPen);

#ifdef PYHT_BG_APPLY_LINEARGRADIENT
    QLinearGradient BgGradient(QPoint(0,0),QPoint(0,height()));
    BgGradient.setColorAt(0.0,PYHT_BG_START_COLOR);
    BgGradient.setColorAt(1.0,PYHT_BG_END_COLOR);
    painter->setBrush(BgGradient);
#else
    painter->setBrush(PYHT_BG_BRUSH_COLOR);
#endif

#ifdef PYHT_SCHEME2
    QLinearGradient Scheme2Gradient(QPoint(0,0),QPoint(0,height()));
    Scheme2Gradient.setColorAt(0.0,PYHT_BG_START_COLOR);
    Scheme2Gradient.setColorAt(1.0,PYHT_BG_END_COLOR);
    painter->setBrush(Scheme2Gradient);
#endif

#ifdef PYHT_SCHEME3
    QLinearGradient Scheme3Gradient(QPoint(0,0),QPoint(0,height()));
    Scheme3Gradient.setColorAt(0.0,PYHT_BG_START_COLOR);
    Scheme3Gradient.setColorAt(1.0,PYHT_BG_END_COLOR);
    painter->setBrush(Scheme3Gradient);
#endif

    painter->drawPath(BgPath);

    QPointF TextPot(PYHT_BG_LEFT_SPACE*2,PYHT_BG_TOP_SPACE*2);


    painter->setPen(PYHT_TEXT_COLOR);

#if 1
    QFont TextFont;
    TextFont.setBold(true);
    painter->setFont(TextFont);
#endif

    painter->drawText(TextPot,m_strTip);


    painter->restore();

    qreal TextLength = fontMetrics().width(m_strTip);
    m_width = PYHT_BG_LEFT_SPACE * 2 + TextLength * 1.5 ;
    m_height = PYHT_BG_TOP_SPACE * 2 + fontMetrics().height();
    setFixedSize(QSize(m_width,m_height));
}


/*!
  public interfaces
*/
PYHistogramTip* PYHistogramTip::getInstance()
{
    if(mInstance == NULL)
    {
        mInstance = new PYHistogramTip;
    }
    return mInstance;
}

void PYHistogramTip::setTip(const QString &str)
{
    m_strTip = str;

    if(m_hideTimer->isActive())
    {
        m_hideTimer->stop();
    }
    m_hideTimer->start();
    update();
}

void PYHistogramTip::setDirection(bool left)
{
    m_bLeft = left;
    update();
}

void PYHistogramTip::setInterval(int sec)
{
    if(m_nInterval == sec)
    {
        return ;
    }

    m_hideTimer->setInterval(m_nInterval * 1000);

}
