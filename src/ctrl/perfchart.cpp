#include "perfchart.h"

#include <QToolTip>
#include <QColorDialog>

#define PERFCHART_SPACE 3
#define X_INCREMENT 5
#define MAX 100

PerfChart::PerfChart(QWidget *parent) :
    QWidget(parent)
{
    initVariables();
    initSettings();
}

void PerfChart::initVariables()
{
    m_nChannelCount = 0;
    m_maxTextLength = 0;
    m_bUseAntialiasing = false;
    m_captionClr = Qt::yellow;
    m_titleClr = Qt::green;
}

void PerfChart::initSettings()
{
    setMouseTracking(true);
}

/*!
  reimplementing protected functions
  */
void PerfChart::paintEvent(QPaintEvent *)
{
    QPainter painter;
    painter.begin(this);

    if(m_bUseAntialiasing)
    {
        painter.setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
    }

    drawVariables(&painter);
    drawBackground(&painter);
    drawTitles(&painter);
    drawText(&painter);
    drawBox(&painter);
    drawGraph(&painter);
}

void PerfChart::mouseMoveEvent(QMouseEvent *e)
{
    for(int i = 0; i < m_rectTips.size(); i++)
    {
        if(m_rectTips.at(i).first.contains(e->pos()))
        {
            QToolTip::showText(e->globalPos(),m_rectTips.at(i).second);
            return;
        }
    }
}

void PerfChart::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)
    {
        if(mouseInRects(e))
        {
            QColor clr = QColorDialog::getColor();
            if(clr.isValid())
            {
                int coreIndex = getClickedCoreIndex(getClickedRectTip(e));
                setChannelColor(coreIndex,clr);
            }
        }
    }
}

/*!
  painting used private functions
  */

void PerfChart::drawVariables(QPainter *painter)
{
    m_topSpace = 10;
    m_leftSpace = 10;
}

void PerfChart::drawBackground(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::NoPen);

    QLinearGradient bgGradient(QPointF(0,0),QPointF(0,height()));
    bgGradient.setColorAt(0.0,QColor(120,120,120));
    bgGradient.setColorAt(0.05,QColor(40,40,40));
    bgGradient.setColorAt(0.95,QColor(40,40,40));
    bgGradient.setColorAt(1.0,QColor(120,120,120));

    painter->setBrush(bgGradient);
    painter->drawRect(rect());

    painter->restore();
}

void PerfChart::drawTitles(QPainter *painter)
{
    qreal vTitleWidth=fontMetrics().width(m_strVTitle);
    qreal vTitleHeight=fontMetrics().height();

    qreal hTitleWidth=fontMetrics().width(m_strHTitle);
    qreal hTitleHeight=fontMetrics().height();

    // draw caption
    painter->save();

    painter->setPen(m_captionClr);
    QFont captionFont;
    captionFont.setFamily("Times new roman");
    //captionFont.setPointSize((int)10);
    captionFont.setBold(true);

    qreal captionWidth=fontMetrics().width(m_strCaption);
    qreal captionHeight=fontMetrics().height();

    painter->setFont(captionFont);
    qreal captionX=width()/2-captionWidth/2;

    QPointF captionPot(captionX,m_topSpace+captionHeight);
    painter->drawText(captionPot,m_strCaption);

    painter->restore();

    // draw vertical title
    painter->save();
    painter->setPen(m_titleClr);

    qreal vTitleY=height()/2-vTitleHeight/2;
    QPointF vTitlePot(m_leftSpace,vTitleY);
    painter->translate(vTitlePot);
    painter->save();
    painter->rotate(-90);
    painter->drawText(QPointF(-vTitleWidth/2,vTitleHeight),m_strVTitle);
    painter->restore();

    painter->restore();

    // draw horizontal title

    painter->save();
    painter->setPen(m_titleClr);
    qreal hTitleX=width()/2-hTitleWidth/2;
    QPointF hTitlePot(hTitleX,height()-m_topSpace);
    painter->drawText(hTitlePot,m_strHTitle);


    painter->restore();

    m_boxTopLeftPot=QPointF(2*m_leftSpace+2*vTitleHeight,m_topSpace+captionHeight*2);
    m_boxBottomRightPot=QPointF(width()-4*m_leftSpace,height()-4*m_topSpace);
    m_boxRect=QRectF(m_boxTopLeftPot,m_boxBottomRightPot);
}

void PerfChart::drawText(QPainter *painter)
{
    m_rectTips.clear();

    qreal startX = m_boxRect.topRight().x() + m_leftSpace;
    qreal startY = m_topSpace;

    const int rectWidth = 20;
    const int rectHeight = 10;

    for(int i=0;i<m_channelTextVec.count();++i)
    {
        QPointF topLeftPot(startX, startY);
        QPointF bottomRightPot(startX + rectWidth,
                               startY + rectHeight);
        QRectF textRect(topLeftPot, bottomRightPot);

        drawItemBox(painter, textRect, m_channelClrVec.at(i));
        m_rectTips.push_back(qMakePair(textRect, m_channelTextVec.at(i)));

        startY += rectHeight + PERFCHART_SPACE;
    }
}

void PerfChart::drawBox(QPainter *painter)
{
    painter->save();
    painter->setPen(QColor(30,140,200));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(m_boxRect);

    int max = 100;
    qreal startY = m_boxRect.topLeft().y();
    qreal startX = m_boxRect.topLeft().x();
    qreal dY = m_boxRect.height()/10;
    QString strValue;
    qreal textWidth = 0;
    qreal textHeight = 0;

    for(int i=0;i<=10;i++)
    {
        strValue = tr("%1%").arg(max);
        textWidth = fontMetrics().width(strValue);
        textHeight = fontMetrics().height();

        QPointF textPot(startX-textWidth-5,startY+textHeight/2);
        painter->drawText(textPot,strValue);

        startY += dY;

        max -= 10;
    }

    painter->setPen(QPen(QColor(30,140,200)));
    qreal hLineY = m_boxRect.topLeft().y()+dY;
    for(int i = 0; i <= 8; i++)
    {
        QPointF leftPot(m_boxRect.topLeft().x(), hLineY);
        QPointF rightPot(m_boxRect.topRight().x(), hLineY);
        painter->drawLine(leftPot, rightPot);
        hLineY += dY;
    }

    painter->restore();
}

void PerfChart::drawGraph(QPainter *painter)
{
    painter->save();

    for(int i=0;i<m_channelDataVec.count();++i)
    {
        qreal startX = m_boxRect.bottomRight().x();

        QVector<QPointF> potVec;

        for(int j = 0; j < m_channelDataVec.at(i).count()-1; ++j)
        {
            qreal value = m_channelDataVec.at(i).at(j);

            qreal dY = (qreal)(m_boxRect.height()/MAX)*value;

            qreal y = m_boxRect.bottomRight().y()-dY;

            qreal value2 = m_channelDataVec.at(i).at(j+1);

            qreal dY2 = (qreal)(m_boxRect.height()/MAX)*value2;

            qreal y2 = m_boxRect.bottomRight().y()-dY2;

            QPointF dataPot(startX,y);
            QPointF dataPot2(startX-X_INCREMENT,y2);

            startX -= X_INCREMENT;

            potVec.push_back(dataPot);
            potVec.push_back(dataPot2);
        }
        painter->setPen(m_channelClrVec.at(i));
        painter->drawLines(potVec);
    }

    painter->restore();
}

void PerfChart::drawItemBox(QPainter* painter, const QRectF &rect, const QColor &clr)
{
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);

    const int Radius = 3;
    QLinearGradient itemGradient(rect.topLeft(),rect.bottomLeft());
    itemGradient.setColorAt(0.0,clr.lighter());
    itemGradient.setColorAt(1.0,clr);
    painter->setBrush(itemGradient);
    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(rect,Radius,Radius);

    painter->restore();
}

bool PerfChart::mouseInRects(QMouseEvent *e)
{
    for(int i = 0; i < m_rectTips.size(); i++)
    {
        if(m_rectTips.at(i).first.contains(e->pos()))
        {
            return true;
        }
    }
    return false;
}

int PerfChart::getClickedCoreIndex(const QString &strTip)
{
    QStringList items = strTip.split(' ');
    return items.at(1).toInt();
}

QString PerfChart::getClickedRectTip(QMouseEvent* e)
{
    for(int i = 0;i < m_rectTips.size(); i++)
    {
        if(m_rectTips.at(i).first.contains(e->pos()))
        {
            return m_rectTips.at(i).second;
        }
    }
    return QString();
}

void PerfChart::addChannelData(int index, qreal data)
{

    if(index>=m_nChannelCount)
    {
        return ;
    }

    int dataCount=m_boxRect.width()/X_INCREMENT;

    m_channelDataVec[index].push_front(data);

    for(int i=0;i<m_channelDataVec.count();++i)
    {
        if(m_channelDataVec.at(i).count()>dataCount)
        {
            m_channelDataVec[i].remove(dataCount,1);
        }
    }
    update();
}
