#include "clearlabel.h"

ClearLabel::ClearLabel(QWidget *parent) :
    QLabel(parent)
{
}

void ClearLabel::mousePressEvent(QMouseEvent *ev)
{
    if(ev->buttons() & Qt::LeftButton)
    {
        emit sig_clicked();
    }

    QLabel::mousePressEvent(ev);
}
