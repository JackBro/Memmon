#ifndef WAITINGWIDGET_H
#define WAITINGWIDGET_H

#include <QWidget>

class QTimer;
class QPaintEvent;
class QPainter;
class QProgressIndicator;
class QLabel;
class QShowEvent;

class WaitingWidget : public QWidget
{
    Q_OBJECT
private:
    explicit WaitingWidget(QWidget *parent = 0);
    virtual ~WaitingWidget();
    
public:
    static void setParent(QWidget* parent);
    static void showText(const QString& strText);
    static void show();
    static void hide();
    static void done();

};


class WaitingLabel : public QWidget
{
    Q_OBJECT
private:
    explicit WaitingLabel(QWidget* parent = 0);

public:
    static WaitingLabel* instance();

    void setText(const QString& strText);

    void setParent(QWidget *parent);

    void done();

protected:
    void paintEvent(QPaintEvent *);

    QSize sizeHint() const
    {
        return QSize(250,80);
    }

    void resizeEvent(QResizeEvent *);
    void showEvent(QShowEvent* e);

private:
    void drawBackground(QPainter* painter);

    void calcGeo();

    void initTimer();
private:
    static WaitingLabel* _instance;
    QWidget* _parent;
    QString _text;
    QProgressIndicator* _indicator;
    QLabel* _label;
    QTimer* _timer;
};


#endif // WAITINGWIDGET_H
