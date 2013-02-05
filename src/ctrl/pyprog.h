#ifndef PYPROG_H
#define PYPROG_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>

class PYProg : public QWidget
{
    Q_OBJECT
public:
    explicit PYProg(QWidget *parent = 0);

public:
    void setRange(qreal min,qreal max);

    void setValue(qreal value);

    void setText(const QString& strText);
    QString text() const;

    void setAnimated(bool animated);
    bool isAnimated() const;

protected:

    void paintEvent(QPaintEvent *);

    QSize sizeHint() const
    {
        return QSize(200,20);
    }

    QSize minimumSizeHint() const
    {
        return QSize(100,20);
    }

    void mousePressEvent(QMouseEvent *);


private:
    void drawBg(QPainter* painter);
    void drawBar(QPainter* painter);

private:
    void initVariables();
    void initAnimationTimer();

private Q_SLOTS:
    void slot_updateValue();

private:
    qreal m_min;
    qreal m_max;
    qreal m_value;
    qreal m_currValue;
    qreal m_dValue;

    QString m_strText;

    bool m_isAnimated;
    bool m_isIncreValue;

    QTimer* m_animationTimer;

Q_SIGNALS:
    void sig_mousePressed();
};

#endif // PYPROG_H
