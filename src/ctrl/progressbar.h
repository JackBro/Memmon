#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QColor>

namespace PB
{
    namespace Color
    {
        const QColor Normal_Start = QColor(232,232,232);
        const QColor Normal_Middle = QColor(250,250,250);
        const QColor Normal_Stop = QColor(240,240,240);

        const QColor Busy_Start = QColor(98,149,21);
        const QColor Busy_Middle = QColor(89,180,16);
        const QColor Busy_Stop = QColor(77,210,34);
    }

    namespace Constant
    {
        const int RoundRadius = 2;
        const int Height = 25;
        const int TextExtraSpace = 5;
    }

    enum TextAlignment { Left,Center,Right };
}

class ProgressBar : public QWidget
{
    Q_OBJECT
public:
    explicit ProgressBar(QWidget *parent = 0);
    
public:

    void setRange(int min,int max);
    void setValue(int value);
    int value() const;
    int minimum() const;
    int maximum() const;

protected:
    void paintEvent(QPaintEvent *);
    QSize sizeHint() const
    {
        return QSize(200,PB::Constant::Height);
    }

private:
    void drawBackground(QPainter* painter);
    void drawForeground(QPainter* painter);
    void drawText(QPainter* painter);
    void setupBrush(QPainter* painter,const QColor& startColor,const QColor& middleColor,const QColor& stopColor);

private:
    int _min,_max,_value;
    PB::TextAlignment _textAlignment;
};

#endif // PROGRESSBAR_H
