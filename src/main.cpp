#include <QApplication>
#include <QLabel>
#include <QTextCodec>
#include "memmon.h"
#include <QMessageBox>
#include "queryengine.h"
#include <QDebug>
#include "logutil.h"
#include "infoquerywindow.h"
#include <QFile>
#include "memchart.h"
#include "testwidget.h"
#include "querythread.h"
#include "xprocesstable/xprocesstable.h"
#include "selectcolumndialog.h"
#include "qradiolist.h"
#include "memorymonitorwindow.h"
#include "3dparty/qtsingleapplication/QtSingleApplication"
#include "xprocesstable/datachart.h"

QString readStyle()
{
    QFile styleFile(":/memmon.qss");
    if(!styleFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(0,"Warning","Failed to load style sheet!");
        return QString();
    }

    return styleFile.readAll();
}

#define TEST_RUN

int main(int argc,char** argv)
{
    QtSingleApplication app(argc,argv);

    if(app.isRunning())
    {
        QMessageBox::warning(0,QObject::tr("Warning"),QObject::tr("Memory Monitor is already running !"));
        exit(0);
    }
    app.setStyleSheet(readStyle());

    QTextCodec::setCodecForTr(QTextCodec::codecForName("Utf8"));

#ifdef TEST_RUN

//    DataChart* win = DataChart::instance();
//    win->setCaption("WorkingsetSize");
//    QList<qreal> values;
//    for(int i = 0;i < 100;i++)
//    {
//        values.push_back(rand()%10000);
//    }
//    win->setData(values);
    Memmon win;
    win.show();

#else

    Memmon win;
    win.show();

#endif
    return app.exec();
}
