#ifndef CLEARLABEL_H
#define CLEARLABEL_H

#include <QLabel>
#include <QMouseEvent>

class ClearLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClearLabel(QWidget *parent = 0);

signals:

public slots:

protected:
    void mousePressEvent(QMouseEvent *ev);

Q_SIGNALS:
    void sig_clicked();
};

#endif // CLEARLABEL_H
