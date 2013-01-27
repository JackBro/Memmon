#include "pyhistory.h"

QBrush GetPromptGradient(const QRectF& rect,const QColor& clr)
{
    QLinearGradient PromptGradient(rect.topLeft(),rect.bottomLeft());
    PromptGradient.setColorAt(0.0,clr.lighter());
    PromptGradient.setColorAt(1.0,clr);
    return PromptGradient;
}

PYHistory::PYHistory(QWidget *parent) :
    QWidget(parent)
{
    initVariables();
}

/*!
  private utility functions
*/
void PYHistory::initVariables()
{
    m_nLevel = 10;
    m_min = 0;
    m_max = 100;
    m_nInterval = 500;
    m_nChannelCount = 0;
    m_nStuffPotCnt = PYH_STUFF_POINT_COUNT;

    m_mode = PERCENTAGE;

    //
    setMouseTracking(true);
    setMinimumSize(minimumSizeHint());
}
#include <QColorDialog>
void PYHistory::mousePressEvent_Handler(QMouseEvent *e)
{
    for(int cIndex = 0;cIndex < m_nChannelCount ; cIndex++)
    {
        if(m_CIVec.at(cIndex)->getRect().contains(e->pos()))
        {
            QColor clr = QColorDialog::getColor();
            if(clr.isValid())
            {
                m_CIVec.at(cIndex)->setColor(clr);
                update();
                return ;
            }
        }
    }
}

#include <QToolTip>
void PYHistory::mouseMoveEvent_Handler(QMouseEvent *e)
{
    bool bIn = false;
    for(int cIndex = 0;cIndex < m_nChannelCount ; cIndex++)
    {
        if(m_CIVec.at(cIndex)->getRect().contains(e->pos()))
        {
            bIn = true;
            break;
        }
    }

    if(bIn)
    {
        QString strTip = tr("click to change color");
        QToolTip::showText(e->globalPos(),strTip,this);
    }

}

/*!
  protected functions
*/
void PYHistory::mousePressEvent(QMouseEvent *e)
{
    EVENT_HANDLER(mousePressEvent)(e);
}

void PYHistory::mouseMoveEvent(QMouseEvent *e)
{
    EVENT_HANDLER(mouseMoveEvent)(e);
}

void PYHistory::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if(PYH_USE_ANTIALIASING)
    {
        painter.setRenderHints(QPainter::Antialiasing|QPainter::HighQualityAntialiasing);
    }
    QFont TextFont("Times New Roman");
    painter.setFont(TextFont);
    drawBg(&painter);
    drawCaption(&painter);
    drawChart(&painter);
    drawPrompt(&painter);
}

/*!
  painting functions
*/
void PYHistory::drawBg(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::NoPen);
    QLinearGradient BgGradient(QPoint(0,0),QPoint(0,height()));
    BgGradient.setColorAt(0.0,PYH_BG_START_COLOR);
    BgGradient.setColorAt(1.0,PYH_BG_END_COLOR);
    painter->setBrush(BgGradient);
    painter->drawRect(rect());
    painter->restore();

}

void PYHistory::drawCaption(QPainter *painter)
{
    painter->save();
    QPointF CaptionTopLeft(PYH_LEFT_SPACE,PYH_TOP_SPACE);
    QPointF CaptionBottomRight(width() - PYH_LEFT_SPACE,PYH_CAPTION_HEIGHT + PYH_TOP_SPACE);
    QRectF CaptionRect(CaptionTopLeft,CaptionBottomRight);

    painter->setPen(PYH_CAPTION_COLOR);
    QFont CaptionFont;
    CaptionFont.setBold(true);
    painter->setFont(CaptionFont);
    painter->drawText(CaptionRect,m_strCaption,PYH_CAPTION_ALIGNMENT);
    painter->restore();
}

void PYHistory::drawChart(QPainter *painter)
{
    painter->save();

    qreal MaxTextLength = 0;

    if(m_mode == PERCENTAGE)
    {
        MaxTextLength = fontMetrics().width(tr("%1%").arg(m_max));
    }else{
        MaxTextLength = fontMetrics().width(tr("%1 %2").arg(m_max).arg(m_strUnit));
    }

    qreal ChartLeft = PYH_LEFT_SPACE + MaxTextLength + PYH_WIDGET_SPACE * 2;
    qreal ChartRight = width() - PYH_LEFT_SPACE;
    qreal ChartTop  = PYH_TOP_SPACE + PYH_CAPTION_HEIGHT + PYH_WIDGET_SPACE;
    qreal ChartBottom = height() - PYH_TOP_SPACE - PYH_CAPTION_HEIGHT * 2 - PYH_WIDGET_SPACE;

    QPointF ChartTopLeft(ChartLeft,ChartTop);
    QPointF ChartBottomRight(ChartRight,ChartBottom);
    QRectF ChartRect(ChartTopLeft,ChartBottomRight);
    painter->setPen(Qt::NoPen);

    QLinearGradient ChartGradient(ChartRect.topLeft(),ChartRect.bottomLeft());
    ChartGradient.setColorAt(0.0,PYH_BG_CHART_START_COLOR);
    ChartGradient.setColorAt(1.0,PYH_BG_CHART_END_COLOR);
    painter->setBrush(ChartGradient);

    painter->drawRect(ChartRect);

    painter->restore();
    /// draw horizontal lines
    painter->save();
    PYH_DONT_USE_ANTIALISING;
    QPen hPen(PYH_CHART_PEN_COLOR,PYH_CHART_PEN_WIDTH,PYH_CHART_PEN_STYLE);
    painter->setPen(hPen);

    qreal dY = (qreal)(ChartBottom - ChartTop)/m_nLevel;
    qreal dValue = (m_max - m_min)/m_nLevel;
    qreal initValue = m_max;
    qreal Top = ChartTop;
    QString strText;
    for(int index = 0;index <= m_nLevel;index++)
    {
        QPointF LeftPot(ChartRect.topLeft().x() - PYH_WIDGET_SPACE,Top);
        QPointF RightPot(ChartRect.topRight().x(),Top);
        painter->drawLine(LeftPot,RightPot);

        /// draw vertical text
        if(m_mode == PERCENTAGE)
        {
            strText = tr("%1%").arg(initValue);
        }else{
            strText = tr("%1 %2").arg(initValue).arg(m_strUnit);
        }
        qreal PotX = LeftPot.x() - fontMetrics().width(strText);
        QPointF VTextPot(PotX,Top + fontMetrics().height()/2);
        painter->drawText(VTextPot,strText);

        /// increment Top
        Top += dY;

        /// decrement initValue
        initValue -= dValue;
    }
    painter->restore();

    /// draw vertical lines
    painter->save();
    PYH_DONT_USE_ANTIALISING;
    QPen vPen(PYH_CHART_PEN_COLOR,PYH_CHART_PEN_WIDTH,PYH_CHART_PEN_STYLE);
    painter->setPen(vPen);

    qreal Left = ChartLeft;
    qreal dX  = (qreal)(ChartRight - ChartLeft)/PYH_TIME_COUNT;
    int FinTime = 60;
    bool bFirst = true;
    for(int index = 0 ;index <= 6;index++)
    {
        QPointF TopPot(Left,ChartRect.topLeft().y());
        QPointF BottomPot(Left,ChartRect.bottomLeft().y());
        painter->drawLine(TopPot,BottomPot);

        // draw

        /// increment Left
        Left += dX;

        // draw horizontal text
        if(bFirst)
        {
            strText = tr("%1 seconds").arg(FinTime);
            bFirst = false;
        }else{
            strText = tr("%1").arg(FinTime);
        }

        QPointF HTextPot(BottomPot.x() - fontMetrics().width(strText)/2,BottomPot.y() + PYH_WIDGET_SPACE + fontMetrics().height());
        painter->drawText(HTextPot,strText);
        /// decrement seconds
        FinTime -= 10;
    }

    painter->restore();

    // draw real stuff

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing|QPainter::HighQualityAntialiasing,false);
    int DataCnt = PYH_TOTAL_TIME/m_nInterval;
    qreal dChartX = (qreal)ChartRect.width()/DataCnt;
    qreal RectHeight = ChartRect.height();
    qreal StartX = ChartRect.topRight().x();
    qreal PotY = 0;
    qreal ChartPotY = 0;
    qreal ChartPotX = 0;
    qreal NextPotY = 0;
    qreal NextChartPotY = 0;
    qreal NextChartPotX = 0;

    //
    qreal dPotX = 0;

    for(int cIndex = 0;cIndex < m_nChannelCount ; cIndex++)
    {
        QVector<qreal> DataVec = m_CIVec.at(cIndex)->getData();
        int DataCnt = DataVec.count();
        StartX = ChartRect.topRight().x();
        QVector<QPointF> PotVec;

        painter->setPen(m_CIVec.at(cIndex)->getColor());

        for(int DataIndex = (DataCnt - 1); DataIndex > 0  ; DataIndex--)
        {
            PotY = (qreal)(RectHeight * DataVec.at(DataIndex)/(m_max - m_min));
            ChartPotY = ChartRect.bottomRight().y() - PotY;
            ChartPotX = StartX;

            QPointF PrevPot(ChartPotX,ChartPotY);
            PotVec.push_back(PrevPot);
            /// decrement StartX
            StartX -= dChartX;

            NextChartPotX = StartX;
            NextPotY = (qreal)(RectHeight  * DataVec.at(DataIndex-1)/(m_max - m_min));
            NextChartPotY = ChartRect.bottomRight().y() - NextPotY;

            //#define PYH_STUFF_POINTS
#ifdef PYH_STUFF_POINTS
            // stuff points
            m_nStuffPotCnt = (ChartPotX - NextChartPotX)/PYH_PIXEL_POINT;

            qreal dStuffPotX = (qreal)(ChartPotX - NextChartPotX)/m_nStuffPotCnt;
            qreal dStuffPotY = 0;
            bool bUp = false;
            if(NextPotY > PotY)
            {
                bUp = true;
                dStuffPotY = (qreal)(NextPotY - PotY)/m_nStuffPotCnt;
            }else{
                dStuffPotY = (qreal)(PotY - NextPotY)/m_nStuffPotCnt;
            }
            qreal StuffPotX = ChartPotX ;
            qreal StuffPotY = ChartRect.bottomRight().y() - PotY ;
            qreal NextStuffPotX = 0;
            qreal NextStuffPotY = 0;

            for(int StuffPotIndex = 0 ;StuffPotIndex < m_nStuffPotCnt; StuffPotIndex++)
            {
                StuffPotX = ChartPotX - dStuffPotX;
                StuffPotY = StuffPotY - dStuffPotY;
                QPointF StuffPot(StuffPotX,StuffPotY);
                PotVec.push_back(StuffPot);

                //
                NextStuffPotX = StuffPotX - dStuffPotX;
                NextStuffPotY = StuffPotY - dStuffPotY;
                QPointF NextStuffPot(NextStuffPotX,NextStuffPotY);
                PotVec.push_back(NextStuffPot);

                // decrement x and value
                StuffPotX -= dStuffPotX;

                if(bUp)
                {
                    StuffPotY += dStuffPotY;
                }else{
                    StuffPotY -= dStuffPotY;
                }
            }
#endif
            QPointF NextPot(NextChartPotX,NextChartPotY);
            PotVec.push_back(NextPot);

        }

        painter->drawLines(PotVec);

    }


    painter->restore();

}

void PYHistory::drawPrompt(QPainter *painter)
{
    painter->save();

    qreal PromptTop = height() - PYH_TOP_SPACE - PYH_CAPTION_HEIGHT;
    qreal PromptLeft  = PYH_LEFT_SPACE;
    qreal PromptIncre = (qreal)(width() - PYH_LEFT_SPACE * 2)/m_nChannelCount;
    qreal PromptRectWidth = PromptIncre/PYH_PROMPT_RECT_FACTOR;

    QString strCaption;
    for(int pIndex = 0; pIndex < m_nChannelCount ; pIndex++)
    {
        QPointF PromptTopLeft(PromptLeft,PromptTop);
        QPointF PromptBottomRight(PromptLeft + PromptRectWidth,PromptTop + PYH_CAPTION_HEIGHT);
        QRectF PromptRect(PromptTopLeft,PromptBottomRight);
        painter->setPen(m_CIVec.at(pIndex)->getColor());
        painter->setBrush(GetPromptGradient(PromptRect,m_CIVec.at(pIndex)->getColor()));
        painter->drawRoundedRect(PromptRect,PYH_RECT_RADIUS,PYH_RECT_RADIUS);

        //
        m_CIVec.at(pIndex)->setRect(PromptRect);

        QPointF PromptTextPot(PromptRect.bottomRight().x() + PYH_WIDGET_SPACE,PromptRect.center().y() + fontMetrics().height()/2);
        QFont TextFont;
        TextFont.setBold(true);
        painter->setFont(TextFont);
        painter->setPen(PYH_PROMPT_TEXT_COLOR);

        if(m_mode == PERCENTAGE)
        {
            qreal value = m_CIVec.at(pIndex)->getLatestData()*100/(m_max - m_min);
            strCaption = tr("%1 (%2%)").arg(m_CIVec.at(pIndex)->getCaption()).arg(value,3,'f',1,'0');
        }else{
            strCaption = tr("%1").arg(m_CIVec.at(pIndex)->getCaption());
        }
        painter->drawText(PromptTextPot,strCaption);

        /// increment initX
        PromptLeft += PromptIncre;
    }


    painter->restore();
}


/*!
  public interfaces
*/
void PYHistory::setCaption(const QString &str)
{
    m_strCaption = str;
    update();
}

void PYHistory::setUnit(const QString &str)
{
    m_strUnit = str;
    update();
}

void PYHistory::setMode(MODE m)
{
    m_mode = m;
    update();
}

QString PYHistory::caption() const
{
    return m_strCaption;
}

void PYHistory::setChannelCount(int count)
{
    m_nChannelCount = count;
    for(int cIndex = 0;cIndex < count;cIndex++)
    {
        ChannelInfo* pCI = new ChannelInfo();

        // generate color automatically
        pCI->setColor(m_CP.GetColor());

        m_CIVec.push_back(pCI);
    }
}

int PYHistory::channelCount() const
{
    return m_nChannelCount;
}

void PYHistory::clear()
{
    int CCnt = m_nChannelCount;
    for(int cIndex = 0;cIndex < CCnt ; cIndex++)
    {
        m_CIVec.at(cIndex)->clear();
    }
    qDeleteAll(m_CIVec.begin(),m_CIVec.end());
    m_CIVec.clear();
}

void PYHistory::render()
{
    update();
}

void PYHistory::setInterval(int interval)
{
    m_nInterval = interval;
}

void PYHistory::addChannelData(int index, qreal data)
{
    PYH_CHECK_INDEX(index);
    int DataCnt = PYH_TOTAL_TIME/m_nInterval;
    for(int cIndex = 0;cIndex < m_nChannelCount ; cIndex++)
    {
        m_CIVec.at(cIndex)->setVisibleDataCount(DataCnt);
        m_CIVec.at(cIndex)->flush();
    }
    m_CIVec.at(index)->addData(data);
    update();
}

void PYHistory::setChannelCaption(int index, const QString &str)
{
    PYH_CHECK_INDEX(index);
    m_CIVec.at(index)->setCaption(str);
}

QString PYHistory::channelCaption(int index) const
{
    PYH_CHECK_INDEX(index);
    return m_CIVec.at(index)->getCaption();
}

void PYHistory::setChannelColor(int index, const QColor &clr)
{
    PYH_CHECK_INDEX(index);
    m_CIVec.at(index)->setColor(clr);
}

QColor PYHistory::channelColor(int index) const
{
    PYH_CHECK_INDEX(index);
    return m_CIVec.at(index)->getColor();
}

void PYHistory::setLevelCount(int level)
{
    m_nLevel = level;
    update();
}

void PYHistory::setRange(qreal min, qreal max)
{
    Q_ASSERT(min < max);
    m_min = min;
    m_max = max;
    update();
}

qreal PYHistory::minimum() const
{
    return m_min;
}

qreal PYHistory::maximum() const
{
    return m_max;
}
