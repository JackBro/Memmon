#ifndef PROCESSINGINDICATOR_H
#define PROCESSINGINDICATOR_H

#include <QWidget>
#include "qt.h"
#define RECT_WIDTH 20
#define LEFT_SPACE 3
#define TOP_SPACE 2
#define RECT_SPACE 3
#define LARGE_ANGLE 20
#define SMALL_ANGLE 30
class ProcessingIndicator : public QWidget
{
Q_OBJECT
public:
    explicit ProcessingIndicator(QWidget *parent = 0);

public Q_SLOTS:
    void start();

    void stop();

    void setInterval(int msec);
protected:
    void paintEvent(QPaintEvent *);
    QSize sizeHint() const
    {
        return QSize(300,70);
    }

private:
    void drawBackground(QPainter* painter);
    void drawRects(QPainter* painter);
private:
    int m_nCurrentIndex;
    int m_nRectCount;
    QTimer* updateTimer;
    bool m_bReverse;

private:
    void initVariables();

private Q_SLOTS:
    void UpdateGraph();
};

#endif // PROCESSINGINDICATOR_H
