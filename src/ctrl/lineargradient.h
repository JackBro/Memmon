#ifndef LINEARGRADIENT_H
#define LINEARGRADIENT_H

#include <QObject>
#include <QLinearGradient>

class LinearGradient : public QObject
{
    Q_OBJECT
public:
    explicit LinearGradient(QObject *parent = 0);

public:
    static QLinearGradient GetGradient(const QPointF& startPos,const QPointF& endPos,const QColor& clr);

};

#endif // LINEARGRADIENT_H
