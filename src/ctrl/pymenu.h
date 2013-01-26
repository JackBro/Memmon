#ifndef PYMENU_H
#define PYMENU_H

#include <QMenu>
#include <QTimer>
#include <QShowEvent>

#define PYM_TIMER_INTERVAL 5
#define PYM_OPACITY_INCREMENT 0.05

class PYMenu : public QMenu
{
    Q_OBJECT
public:

    explicit PYMenu(QWidget *parent = 0);
    explicit PYMenu(const QString& str,QWidget* parent = 0);

protected:

    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);
    virtual void closeEvent(QCloseEvent *);

private:
    QTimer* fadeTimer;

    qreal m_opacity;

private slots:
    void DoFading();

Q_SIGNALS:
    void sig_aboutToShow();
};

#endif // PYMENU_H
