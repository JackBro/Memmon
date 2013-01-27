#ifndef PYHISTORY_H
#define PYHISTORY_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

#include "colorpicker.h"

#define PYH_STUFF_POINT_COUNT 20
#define PYH_TOP_SPACE 10
#define PYH_LEFT_SPACE 10
#define PYH_WIDGET_SPACE 7
#define PYH_CAPTION_HEIGHT 17
#define PYH_PROMPT_RECT_FACTOR 7
#define PYH_RECT_RADIUS 3
#define PYH_PIXEL_POINT 2

#define PYH_BG_SCHEME3

#ifdef PYH_BG_SCHEME1

#define PYH_BG_START_COLOR QColor(121,121,121)
#define PYH_BG_END_COLOR QColor(89,89,89)

#define PYH_BG_CHART_START_COLOR QColor(211,211,211)
#define PYH_BG_CHART_END_COLOR QColor(189,189,189)

#define PYH_PROMPT_TEXT_COLOR Qt::white

#endif

#ifdef PYH_BG_SCHEME2

#define PYH_BG_START_COLOR QColor(145,145,145)
#define PYH_BG_END_COLOR QColor(145,145,145)

#define PYH_BG_CHART_START_COLOR QColor(211,211,211)
#define PYH_BG_CHART_END_COLOR QColor(189,189,189)

#define PYH_PROMPT_TEXT_COLOR Qt::white

#endif

#ifdef PYH_BG_SCHEME3

#define PYH_BG_START_COLOR QColor(212,212,212)
#define PYH_BG_END_COLOR QColor(189,189,189)

#define PYH_BG_CHART_START_COLOR Qt::white
#define PYH_BG_CHART_END_COLOR Qt::white

#define PYH_PROMPT_TEXT_COLOR Qt::black

#endif



#define PYH_CAPTION_COLOR Qt::black
#define PYH_CAPTION_ALIGNMENT Qt::AlignVCenter|Qt::AlignLeft

#define PYH_CHECK_INDEX(index)   Q_ASSERT(index >= 0 && index < m_nChannelCount);
#define PYH_USE_ANTIALIASING true

#define EVENT_HANDLER(Event_Func) Event_Func##_Handler

#define PYH_CHART_PEN_COLOR Qt::black
#define PYH_CHART_PEN_STYLE Qt::DotLine
#define PYH_CHART_PEN_WIDTH 1

#define PYH_TIME_COUNT 6
#define PYH_DONT_USE_ANTIALISING painter->setRenderHints(QPainter::Antialiasing|QPainter::HighQualityAntialiasing,false);
#define PYH_TOTAL_TIME 60000



#include <QDebug>
#ifdef DEBUG_VAR
#undef DEBUG_VAR
#endif
#define DEBUG_VAR(VAR) qDebug() << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << #VAR << ":" <<  VAR;

class ChannelInfo
{
public:
  ChannelInfo():m_nVisibleDataCount(0){}

public:
  /// public interfaces
  void setCaption(const QString& str)
  {
    m_strCaption = str;
  }

  QString getCaption() const
  {
    return m_strCaption;
  }

  void setColor(const QColor& clr)
  {
    m_clr = clr;
  }

  QColor getColor() const
  {
    return m_clr;
  }

  void addData(qreal data)
  {
    m_dataVec.push_back(data);
  }

  QVector<qreal> getData() const
  {
    return m_dataVec;
  }

  int getDataCount() const
  {
    return m_dataVec.count();
  }

  void setVisibleDataCount(int count)
  {
    m_nVisibleDataCount = count;
  }

  void flush()
  {
    if(m_dataVec.count() > m_nVisibleDataCount)
    {
      int ExtraCnt = m_dataVec.count() - m_nVisibleDataCount;
      m_dataVec.remove(0,ExtraCnt);
    }
  }

  qreal  getLatestData() const
  {
    if(m_dataVec.count() > 0)
    {
      return m_dataVec.at(m_dataVec.count() - 1);
    }else{
      return 0;
    }
  }
  void setRect(const QRectF& rect)
  {
    m_rect = rect;
  }

  QRectF getRect() const
  {
    return m_rect;
  }

  void clear()
  {
    m_dataVec.clear();
  }


private:
  QString         m_strCaption;
  QColor          m_clr;
  QVector<qreal>  m_dataVec;
  QRectF          m_rect;
  int             m_nVisibleDataCount;
};

class PYHistory : public QWidget
{
    Q_OBJECT
public:
    enum MODE { PERCENTAGE , VALUE };
    explicit PYHistory(QWidget *parent = 0);

public:
    /// public interfaces

    void setMode(MODE m);

    void setCaption(const QString& str);

    void setUnit(const QString& str);

    QString caption() const;

    void setChannelCount(int count);

    int channelCount() const;

    void addChannelData(int index,qreal data);

    void setChannelCaption(int index,const QString& str);

    QString channelCaption(int index) const;

    void setChannelColor(int index,const QColor& clr);

    QColor channelColor(int index) const;

    void setLevelCount(int level);

    int levelCount() const;

    void setRange(qreal min,qreal max);

    qreal minimum() const;

    qreal maximum() const;

    void clear();

    void render();

    void setInterval(int interval); // ms

protected:
    ///
    void paintEvent(QPaintEvent *);


    QSize sizeHint() const
    {
      return QSize(500,300);
    }

    QSize minimumSizeHint() const
    {
      return QSize(200,200);
    }

    void mousePressEvent(QMouseEvent *);

    void mouseMoveEvent(QMouseEvent *);

private:
    void drawBg(QPainter* painter);

    void drawCaption(QPainter* painter);

    void drawChart(QPainter* painter);

    void drawPrompt(QPainter* painter);


private:
    void initVariables();

    void mousePressEvent_Handler(QMouseEvent* e);

    void mouseMoveEvent_Handler(QMouseEvent* e);

private:

    QString m_strCaption;
    QString m_strUnit;

    int     m_nChannelCount;
    int     m_nLevel;
    int     m_nInterval;
    int     m_nStuffPotCnt;

    qreal   m_min;
    qreal   m_max;

    MODE    m_mode;

    ColorPicker m_CP;

    QVector<ChannelInfo*> m_CIVec;
};

QBrush GetPromptGradient(const QRectF& rect,const QColor& clr);
#endif // PYHISTORY_H
