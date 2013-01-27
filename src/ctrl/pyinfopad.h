#ifndef PYINFOPAD_H
#define PYINFOPAD_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

class CategoryInfo;
class PYInfoPad : public QWidget
{
    Q_OBJECT
public:
    explicit PYInfoPad(QWidget *parent = 0);

public:

    void AddCategory(const QString& strCaption);
    int CategoryCount() const;

    void AddItem(int CategoryIndex,QPair<QString,QString> ItemPair);
    void Clear();

    void SetIconVisible(bool visible);
    bool IsIconVisible() const;

    void SetCategoryIcon(int CategoryIndex, const QIcon& icon);
    QIcon GetCategoryIcon(int CategoryIndex) const;

    void Render();

protected:

    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

private:

    void drawBg(QPainter* painter);
    QRectF drawHeader(QPainter* painter,qreal initY);
    bool drawCheckBox(QPainter* painter,const QRectF& HeaderRect, qreal initY, int CateIndex);
    void drawSubItems(QPainter* painter,qreal& initY,int CateIndex);
    void drawCategory(QPainter* painter);

    void updateAllItemsVisibility(bool visible);

private:

    QVector<CategoryInfo*> m_CategoryInfoVec;
    qreal m_height;
    bool m_bIconVisible;

private:
    void MousePressHandler(QMouseEvent* );

};

#endif // SYSTEMINFOWIDGET_H
