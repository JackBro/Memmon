#ifndef SEARCHEDIT_H
#define SEARCHEDIT_H

#include <QWidget>
#include <QResizeEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QLabel>
#include <QToolButton>
#include <QLineEdit>

#include "clearlabel.h"

class SearchEdit : public QWidget
{
    Q_OBJECT
public:
    explicit SearchEdit(QWidget *parent = 0);

public:

    void setText(const QString& str);
    void clear();
    QString text() const;
    QLineEdit* lineEdit() const;
    void focus();

protected:

    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    QSize sizeHint() const
    {
        return QSize(300,40);
    }

    QSize minimumSizeHint() const
    {
        return QSize(120,40);
    }

    void showEvent(QShowEvent *)
    {
        emit sig_isVisible(true);
    }

    void hideEvent(QHideEvent *)
    {
        emit sig_isVisible(false);
    }


private:

    void drawBackground(QPainter* paitner);

private:

    void calcGeo();
    void initVariables();
    void initWidgets();

private:
    QLabel* m_searchLabel;
    ClearLabel* m_clearLabel;
    QLineEdit* m_searchEdit;

    bool m_bEnter;

private Q_SLOTS:

    void HandleTextChanged(const QString& str);
    void ClearText();

Q_SIGNALS:
    void textChanged(const QString& str);
    void sig_isVisible(bool visible);

};

#endif // SEARCHEDIT_H
