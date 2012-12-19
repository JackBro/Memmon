#ifndef DATACHART_H
#define DATACHART_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>
#include <QPushButton>

namespace NS_DataChart
{
    namespace Constant
    {
        const int CaptionHeight = 20;
        const int LittleRectWidth = 2;
        const int AngleEdgeLength = 3;
        const int BottomSpace = 20;
        const int ExtraSpace = 3;
        const int VerticalGridCount = 2;
        const int LeftSpace = 20;
        const int StayDuration = 10000; // 10 seconds
    }

    namespace Color
    {
        const QColor BackgroundColor = QColor(153,153,153);
        const QColor CaptionColor = Qt::white;

        const QColor ChartStartColor = QColor(243,243,243);
        const QColor ChartStopColor = QColor(211,211,211);

        const QColor LegendColor = QColor(78,78,78);

        const QColor LittleRectColor = QColor(88,88,88);
        const QColor GraphColor = QColor(111,111,111);
    }
}
class DataChartCaption;
class DataChartPrivate;
class DataChart : public QWidget
{
    Q_OBJECT
private:
    explicit DataChart(QWidget *parent = 0);
    
public:
    static DataChart* instance();

    void setData(const QList<qreal>& data);
    void clear();

    void showAt(const QPoint& showPoint);

    void setCaption(const QString& strCaption);
    QString caption() const;

protected:
    QSize sizeHint() const
    {
        return QSize(300,300);
    }

    void showEvent(QShowEvent *);

    void mousePressEvent(QMouseEvent *);

    void mouseMoveEvent(QMouseEvent *);


private:
    static DataChart* _instance;

protected:

private:
    void setThisSize();
    void initTimer();
    void resetTimer();

private:
    QPushButton* _pinButton;
    QPushButton* _closeButton;
    DataChartPrivate* _d;
    DataChartCaption* _caption;

    QTimer* _hideTimer;
    bool _pressed;
    QPoint _offset;

private Q_SLOTS:
    void slot_pinThis();
};

class DataChartCaption : public QWidget
{
    Q_OBJECT
private:
    explicit DataChartCaption(QWidget* parent = 0);

public:
    void setCaption(const QString& strCaption);
    QString caption() const;

protected:
    void paintEvent(QPaintEvent *);

private:
    void draw(QPainter* painter);

private:
    QString _strCaption;

    friend class DataChart;
};

class DataChartPrivate : public QWidget
{
    Q_OBJECT
public:
    explicit DataChartPrivate(QWidget* parent = 0);

public:
    void setData(const QList<qreal>& data);
    void clear();

protected:
    void paintEvent(QPaintEvent *);

private:
    void drawBackground(QPainter* painter);
    void drawLegend(QPainter* painter);
    void drawGraph(QPainter* painter);
    void drawLittleAngle(QPainter* painter,const QPoint& vertex,Qt::Orientation o);
    void drawRotatedLittleRect(QPainter* painter,const QPoint& center);

    void findMaxValue();

private:
    typedef QPair<QRectF,qreal> RectValue;
    QList<qreal> _data;
    qreal _maxValue;
    qreal _maxLeftSpace;

};

#endif // DATACHART_H
