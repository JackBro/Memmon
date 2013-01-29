#include "lineargradient.h"

#define INCREMENT 66

LinearGradient::LinearGradient(QObject *parent) :
    QObject(parent)
{

}

QLinearGradient LinearGradient::GetGradient(const QPointF &startPos, const QPointF &endPos, const QColor &clr)
{
    QLinearGradient gradient(startPos,endPos);
    int red=clr.red();
    int green=clr.green();
    int blue=clr.blue();

    int startRed=red-INCREMENT>=0 ? red-INCREMENT : 0;
    int startGreen=green-INCREMENT>0 ? green-INCREMENT : 0;
    int startBlue=blue-INCREMENT >0? blue-INCREMENT : 0;

    QColor startClr(startRed,startGreen,startBlue);

    int endRed=red+INCREMENT < 255 ? red+INCREMENT : 255;
    int endGreen= green+INCREMENT < 255 ? green+INCREMENT : 255 ;
    int endBlue= blue+INCREMENT < 255 ? blue+INCREMENT : 255;

    QColor endClr(endRed,endGreen,endBlue);

    gradient.setColorAt(0.0,endClr);
    gradient.setColorAt(0.5,clr);
    gradient.setColorAt(1.0,endClr);

    return gradient;
}
