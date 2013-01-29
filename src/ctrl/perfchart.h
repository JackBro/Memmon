#ifndef PERFCHART_H
#define PERFCHART_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

#include "lineargradient.h"
#include "colorpicker.h"

class PerfChart : public QWidget
{
    Q_OBJECT
public:
    explicit PerfChart(QWidget *parent = 0);

public:

public slots:
    //
    void setCaptionColor(const QColor& clr)
    {
        m_captionClr=clr;
        update();
    }

    void setTitleColor(const QColor& clr)
    {
        m_titleClr=clr;
        update();
    }

    void setChannelCount(int cnt)
    {
        if(cnt >= 0)
        {
            m_nChannelCount=cnt;

            m_channelDataVec.resize(cnt);
            m_channelTextVec.resize(cnt);

            for(int i=0;i<cnt;++i)
            {
                m_channelClrVec.push_back(m_cp.GetColor());
            }
            update();
        }
    }

    void setChannelText(int index,const QString& str)
    {
        if(index>=m_nChannelCount)
        {
            return ;
        }
        m_channelTextVec[index]=str;

        //
        qreal textWidth=fontMetrics().width(str);

        if(textWidth>m_maxTextLength)
        {
            m_maxTextLength=textWidth;
        }

        update();
    }

    //
    void setChannelColor(int index,const QColor& clr)
    {
        if(index>=m_nChannelCount)
        {
            return ;
        }

        m_channelClrVec[index]=clr;
    }

    void addChannelData(int index,qreal data);

    void setCaption(const QString& str)
    {
        m_strCaption=str;
        update();
    }

    void setVerticalTitle(const QString& str)
    {
        m_strVTitle=str;
        update();
    }

    void setHorizontalTitle(const QString& str)
    {
        m_strHTitle=str;
        update();
    }

    void setUseAntialiasing(bool use)
    {
        m_bUseAntialiasing=use;
        update();
    }


protected:
    void paintEvent(QPaintEvent *);

    QSize sizeHint() const
    {
        return QSize(400,400);
    }

    QSize minimumSizeHint() const
    {
        return QSize(300,300);
    }

    /*!
      painting used private functions
      */
private:
    void drawVariables(QPainter* painter);

    void drawBackground(QPainter* painter);

    void drawTitles(QPainter* painter);

    void drawText(QPainter* painter);

    void drawBox(QPainter* painter);

    void drawGraph(QPainter* painter);


private:

    int     m_nChannelCount;
    QVector< QVector<qreal> >m_channelDataVec;
    QVector<QColor> m_channelClrVec;
    QVector<QString> m_channelTextVec;

    QString m_strVTitle;
    QString m_strHTitle;
    QString m_strCaption;

    QPointF m_boxTopLeftPot;
    QPointF m_boxBottomRightPot;
    QRectF  m_boxRect;

    QColor  m_captionClr;
    QColor  m_titleClr;

    qreal m_topSpace;
    qreal m_leftSpace;
    qreal m_boxWidth;
    qreal m_boxHeight;
    qreal m_maxTextLength;

    ColorPicker m_cp;
    LinearGradient m_lg;

    bool  m_bUseAntialiasing;

    /*!

      */
private:
    void initVariables();

};

#endif // PERFCHART_H
