#ifndef MEMCHART_H
#define MEMCHART_H

#include <QWidget>
#include <qglobal.h>
#include <QObject>

class MemChartPrivate;
class MemChart : public QWidget
{
    Q_DECLARE_PRIVATE(MemChart)
    Q_OBJECT
public:
    explicit MemChart(QWidget *parent = 0);

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

private:
    MemChartPrivate* d_ptr;
    
    Q_DISABLE_COPY(MemChart)
};

#endif // MEMCHART_H
