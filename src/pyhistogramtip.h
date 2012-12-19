#ifndef PYHISTOGRAMTIP_H
#define PYHISTOGRAMTIP_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>

#define PYHT_BG_BRUSH_COLOR Qt::white
#define PYHT_BG_PEN_COLOR Qt::blue
#define PYHT_BG_PEN_WIDTH 1
#define PYHT_BG_LEFT_SPACE 10
#define PYHT_BG_TOP_SPACE 10
#define PYHT_TEXT_COLOR Qt::black

//#define PYHT_BG_APPLY_LINEARGRADIENT
#ifdef PYHT_BG_APPLY_LINEARGRADIENT
#define PYHT_BG_START_COLOR QColor(248,170,13)
#define PYHT_BG_END_COLOR QColor(231,158,12)
#endif

#ifdef PYHT_TEXT_COLOR
#undef PYHT_TEXT_COLOR
#endif

#define PYHT_SCHEME3
#ifdef PYHT_SCHEME2
#define PYHT_TEXT_COLOR Qt::white
#define PYHT_BG_START_COLOR QColor(120,120,120)
#define PYHT_BG_END_COLOR QColor(50,50,50)

#endif

#ifdef PYHT_SCHEME3
#define PYHT_TEXT_COLOR Qt::black
#define PYHT_BG_START_COLOR QColor(52,191,255)
#define PYHT_BG_END_COLOR QColor(40,149,199)
#endif

class PYHistogramTip : public QWidget
{
    Q_OBJECT
private:
    explicit PYHistogramTip(QWidget *parent = 0);


public:

    static PYHistogramTip* getInstance();

    void setTip(const QString& str);

    void setDirection(bool left);

    void setInterval(int sec);


protected:
    void paintEvent(QPaintEvent *);

    QSize sizeHint() const
    {
        return QSize(100,30);
    }

private:
    void draw(QPainter* painter);



private:

    QString m_strTip;

    qreal m_width;
    qreal m_height;

    bool  m_bLeft;

    int m_nInterval;

    QTimer* m_hideTimer;

    static PYHistogramTip* mInstance;

private:
    void initVariables();
    void initSettings();
};

#endif // PYHISTOGRAMTIP_H
