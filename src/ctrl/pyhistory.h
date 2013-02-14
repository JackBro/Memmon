#ifndef PYHISTORY_H
#define PYHISTORY_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

#include "colorpicker.h"

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
