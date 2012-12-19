#include <QApplication>
#include "../memchart.h"
int main(int argc,char** argv)
{
    QApplication app(argc,argv);
    MemChart win;
    win.show();
    return app.exec();
}
