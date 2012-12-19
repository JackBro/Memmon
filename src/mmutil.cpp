#include "mmutil.h"
#include <QObject>
namespace MM_Util
{
    #define TAIL 0,'g',3,'0'
    QString getDecentSize(qreal size)
    {
        if(size < 1024)
        {
            return QObject::tr("%1 B").arg(size);
        }
        else if(size > 1024 && size < 1024*1024)
        {
            return QObject::tr("%1 KB").arg((qreal)size/1024,TAIL);
        }
        else if(size > 1024*1024 && size < 1024*1024*1024)
        {
            return QObject::tr("%1 MB").arg((qreal)size/(1024*1024),TAIL);
        }
        else if(size > 1024*1024*1024)
        {
            return QObject::tr("%1 GB").arg((qreal)size/(1024*1024*1024),TAIL);
        }
    }

    #undef TAIL


}
