#ifndef MEMCHART_P_H
#define MEMCHART_P_H

#include <QWidget>
#include <QObject>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QAction>
#include <QMenu>

namespace MC
{

    namespace Color
    {
        const QColor StartBackground = QColor(221,221,221);
        const QColor StopBackground = QColor(99,99,99);

        const QColor HistogramStart = QColor(71,160,255);
        const QColor HistogramStop = QColor(64,144,230);
        const QColor HistogramPen = HistogramStart;

        const QColor CurveColor = QColor(189,244,85);

        const QColor HighlightColor = QColor(183,150,254);
        const QColor LegendColor = QColor(45,45,45);

        const QColor TextColor = LegendColor;
        const QColor CaptionColor = Qt::black;
    }

    namespace Constant
    {
        const int MinHistogramWidth = 10;
        const int MaxHistogramWidth = 50;
        const int MinHistogramSpace = 10;
        const int TopSpace = 15;
        const int ExtraSpace = 3;
        const int GridCount = 5;
        const float MaxRatio = 1.3333;
        const int CurvePenWidth = 2;
        const int FrameWidth = 1;
    }
}

class MemChart;
class MemChartPrivate : public QWidget
{
    Q_DECLARE_PUBLIC(MemChart)
    Q_OBJECT
public:
    explicit MemChartPrivate(QWidget* parent = 0);

public:
    void setCaption(const QString& strCaption);
    QString caption() const;

    void setCaptionAlignment(Qt::Alignment alignment);
    Qt::Alignment captionAlignment() const;

    void addValue(int value);

    void setCurveVisible(bool visible);
    bool isCurveVisible() const;

    void setHistogramVisible(bool visible);
    bool isHistogramVisible() const;

    void clear();


protected:
    void paintEvent(QPaintEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    QSize sizeHint() const
    {
        return QSize(600,400);
    }

    QSize minimumSizeHint() const
    {
        return QSize(300,200);
    }

    void resizeEvent(QResizeEvent *);
    void contextMenuEvent(QContextMenuEvent *);
    void leaveEvent(QEvent *);

private:
    void drawBackground(QPainter* painter);
    void drawCaption(QPainter* painter);
    void drawFrame(QPainter* painter);
    void drawLegend(QPainter* painter);
    void drawLegendText(QPainter* painter,qreal initX);
    void drawHistogram(QPainter* painter);
    void drawSelectedRegion(QPainter* painter);

    QBrush getBrush(const QRectF& rect,Qt::Orientation o,const QColor& startColor,const QColor& stopColor);
    void drawRect(QPainter* painter,const QRectF& rect,Qt::Orientation o,const QColor& startColor,const QColor& stopColor);

    void initVars();
    void initSettings();
    void showHighlightTip();
    QPointF constructPoint(QMouseEvent* e);
    void adjustElementCount();
    void initContextMenu();
    void showFancyTip(const QPointF& globalPoint,const QString& strTip);
    void resetPoints();

private:

    typedef QPair<QRectF,QString> BarItem;
    MemChart* q_ptr;

    bool _isCurveVisible;
    bool _isHistogramVisible;
    bool _mousePressed;
    bool _isAcceptData;

    int _min;
    int _max; // the max of legend
    int _maxValue; // the max of values
    int _histogramWidth;

    QPointF _pressedPoint;
    QPointF _movePoint;

    QVector<int> _values;
    QVector<BarItem> _barItems;

    QMenu* _contextMenu;
    QAction* _showCurveAct;
    QAction* _showHistogramAct;
    QAction* _acceptDataAct;

    QString _caption;
    Qt::Alignment _alignment;

private Q_SLOTS:
    void ActionHandler();

private:
    Q_DISABLE_COPY(MemChartPrivate)

};

#endif // MEMCHART_P_H
