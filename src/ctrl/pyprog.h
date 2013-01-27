#ifndef PYPROG_H
#define PYPROG_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

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
    /// painting functions
    void drawBg(QPainter* painter);

    void drawBar(QPainter* painter);



private:
    void initVariables();

private:
    qreal m_min;
    qreal m_max;
    qreal m_value;

    QString m_strText;

Q_SIGNALS:
    void sig_mousePressed();
};

#endif // PYPROG_H
