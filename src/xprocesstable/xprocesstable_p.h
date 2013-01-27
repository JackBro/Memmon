#ifndef XPROCESSTABLE_P_H
#define XPROCESSTABLE_P_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QScrollArea>

#ifndef Container
#define Container QVector
#endif

#undef GET_SENDER
#define GET_SENDER(OBJECT) OBJECT* who = qobject_cast<OBJECT *>(sender());

#undef DEFINE_STRING
#define DEFINE_STRING(VAR,VALUE) const QString VAR = VALUE;

namespace XPT
{
    namespace Color
    {
        const QColor PenColor = QColor(89,89,89);
        const QColor LightColor = QColor(233,233,233);
        const QColor DarkColor = QColor(198,198,198);
        const QColor ColumnLineColor = QColor(132,132,132);
        const QColor HighlightColor = QColor(255,203,44);

        const QColor ExpandBoxStart = QColor(241,241,241);
        const QColor ExpandBoxStop = QColor(221,221,221);
        const QColor ExpandBoxPen = QColor(111,111,111);

        const QColor TextColor = QColor(56,56,56);
    }

    namespace Constant
    {
        const int TinySpace = 1;
        const int RowHeight = 20;
        const int ExtraSpace = 3;
        const int LeftSpace = 5;
        const int BoxExtraSpace = 3;
        const QSize IconSize = QSize(18,18);
        const QSize ExpandBoxSize = QSize(10,10);
        const int IconExtraSpace = 1;

        const float HighlightBarOpacity = 0.6;
    }

    namespace String
    {
        DEFINE_STRING(CM_AutoAdjust,"Auto Adjust")
    }
}
class XProcessTable;
class XProcessItem;

class XScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    XScrollArea(QWidget* parent = 0);

private Q_SLOTS:
    void hScrollBarValueChanged(int value);
    void vScrollBarValueChanged(int value);
    void layoutVScrollBar(int hValue);

protected:
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *e);
private:
    int _startX;
    int _startY;

Q_SIGNALS:
    void setViewportX(int startX,int endX);
    void setViewportY(int startY,int endY);
    void passKeyPressEvent(QKeyEvent* e);
};

class XProcessTablePrivate : public QWidget
{
    Q_OBJECT
public:
    explicit XProcessTablePrivate(QWidget* parent = 0);


public:
    void addItem(XProcessItem* item);

    void clear();

    void removeItemByPid(uint32_t pid);

    QByteArray contents();

protected:
    void paintEvent(QPaintEvent* e);
    void mousePressEvent(QMouseEvent* e);

    QSize sizeHint() const
    {
        return QSize(400,400);
    }

    void resizeEvent(QResizeEvent* e);

private:
    void drawBackground(QPainter* painter);
    void drawColumnLines(QPainter* painter);
    void drawHighlightBar(QPainter* painter);
    void drawItems(QPainter* painter);
    void drawItem(QPainter* painter,XProcessItem* item,qreal initX,qreal& initY);
    void drawChildItems(QPainter* painter,const Container<XProcessItem*>& children);

    void relayout();
    void layoutWidgets(const Container<XProcessItem*> items,qreal& initY);
    void layoutWidget(XProcessItem* item,qreal& initY);
    QRectF getExpandBox(qreal initX,qreal initY);

    void drawExpandBox(QPainter* painter,const QRectF& expandboxRect,XProcessItem* item);
    void drawExpandBoxHandles(QPainter* painter,const QRectF& expandboxRect,bool expand);
    void drawIcon(QPainter* painter,qreal initX,qreal initY,XProcessItem* item);
    void drawText(QPainter* painter,qreal initX,qreal initY,XProcessItem* item);

    QString getDottedString(const QString &originalStr, qreal width);

private:
    typedef QPair<QRectF,XProcessItem*> RectItem;
    QVector<qreal> _allColumnWidth;
    Container<XProcessItem*> _items;
    QVector<RectItem> _rectItems;
    int _height;

    QPointF _pressedPoint;

    int _startX;
    int _stopX;
    int _startY;
    int _stopY;


private Q_SLOTS:
    void columnWidthChanged(QVector<qreal> allColumnWidth);
    void setViewportX(int x1,int x2);
    void setViewportY(int y1,int y2);


    friend class XProcessTable;
};

#endif // XPROCESSTABLE_P_H
