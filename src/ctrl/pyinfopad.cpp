#include "pyinfopad.h"

#include <QIcon>
#include <QDebug>

#define SIW_HEADER_HEIGHT 25
#define SIW_ITEM_HEIGHT 20
#define SIW_ICON_SIZE QSize(17,17) // height - extraWidth
#define SIW_BOX_WIDTH 10
#define SIW_BOX_WIDTH_EXTRA 3
#define SIW_EXTRA_SPACE 3
#define SIW_BOX_HALF_WIDTH ((SIW_BOX_WIDTH - 2 * SIW_BOX_WIDTH_EXTRA)/2)
#define SIW_BOX_TEXT_SPACE 4
#define SIW_LEFT_SPACE 10

#define SIW_BG_START_COLOR QColor(227,227,227)
#define SIW_BG_END_COLOR QColor(214,214,214)
#define SIW_BOX_START_COLOR QColor(250,250,250)
#define SIW_BOX_END_COLOR QColor(210,210,210)
#define SIW_HEADER_START_COLOR QColor(241,241,241)/*QColor(0,170,255)*/
#define SIW_HEADER_END_COLOR QColor(218,218,218)/*QColor(0,153,229)*/
#define SIW_BOX_PEN_COLOR Qt::black
#define SIW_PROMPT_PEN_COLOR  QColor(70,70,70)
#define SIW_VALUE_PEN_COLOR QColor(30,30,30)
#define SIW_CATEGORY_COLOR QColor(30,30,30)

#define SIW_CONTENT_START_COLOR  QColor(221,221,221)
#define SIW_CONTENT_END_COLOR    QColor(208,208,208)
#define SIW_CONTENT_FRAME_COLOR QColor(56,56,56)

#define SIW_PROMPT_ALIGNMENT Qt::AlignVCenter|Qt::AlignHCenter
#define SIW_VALUE_ALIGNMENT Qt::AlignVCenter|Qt::AlignLeft


#ifdef SIW_APPLY_HEADER_GRADIENT
#undef SIW_APPLY_HEADER_GRADIENT
#endif
#define SIW_APPLY_HEADER_GRADIENT(Rect) QLinearGradient HeaderGradient(Rect.topLeft(),Rect.bottomLeft()); \
    HeaderGradient.setColorAt(0.0,SIW_HEADER_START_COLOR); \
    HeaderGradient.setColorAt(1.0,SIW_HEADER_END_COLOR); \
    painter->setBrush(HeaderGradient);


class CategoryInfo
{
public:
    explicit CategoryInfo(const QString& strCaption):m_strCaption(strCaption),m_bExpand(true),m_bShowIcon(true){}
    explicit CategoryInfo():m_bExpand(true),m_bShowIcon(true){}

public:
    void SetExpand(bool expand)
    {
        m_bExpand = expand;
    }

    bool IsExpand() const
    {
        return m_bExpand;
    }

    void SetRect(const QRectF& rect)
    {
        m_rect = rect;
    }

    QRectF GetRect() const
    {
        return m_rect;
    }

    void AddItem(const QPair<QString,QString>& ItemPair)
    {
        m_ItemPairVec.push_back(ItemPair);
    }

    void Clear()
    {
        m_ItemPairVec.clear();
    }

    void SetCaption(const QString& strCaption)
    {
        m_strCaption = strCaption;
    }

    QString GetCaption() const
    {
        return m_strCaption;
    }

    QVector< QPair<QString,QString> > GetData() const
    {
        return m_ItemPairVec;
    }

    void SetIcon(const QIcon& icon)
    {
        m_icon = icon;
    }

    QIcon GetIcon() const
    {
        return m_icon;
    }

    void SetIconVisible(bool visible)
    {
        if(m_bShowIcon == visible)
        {
            return;
        }
        m_bShowIcon = visible;
    }

    bool IsIconVisible() const
    {
        return m_bShowIcon;
    }

private:

    QString m_strCaption;

    bool m_bExpand;
    bool m_bShowIcon;

    QRectF m_rect;

    QVector<QPair<QString,QString> > m_ItemPairVec;

    QIcon m_icon;
};
PYInfoPad::PYInfoPad(QWidget *parent) :
    QWidget(parent)
{
    m_height = sizeHint().height();
    m_bIconVisible = true;
}

/*!
  private utility functions
*/

void PYInfoPad::MousePressHandler(QMouseEvent *e)
{
    int CateCnt = m_CategoryInfoVec.count();
    for(int CateIndex = 0 ;CateIndex < CateCnt ; CateIndex++)
    {
        if(m_CategoryInfoVec.at(CateIndex)->GetRect().contains(e->pos()))
        {
            if(m_CategoryInfoVec.at(CateIndex)->IsExpand())
            {
                m_CategoryInfoVec.at(CateIndex)->SetExpand(false);
            }
            else
            {
                m_CategoryInfoVec.at(CateIndex)->SetExpand(true);
            }
            Render();
            return ;
        }
    }
}

/*!
  reimplementing protected functions
*/
void PYInfoPad::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    drawBg(&painter);
    drawCategory(&painter);
}

void PYInfoPad::mousePressEvent(QMouseEvent *e)
{
    MousePressHandler(e);
}

/*!
  painting functions
*/
void PYInfoPad::drawBg(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::white);
    painter->drawRect(rect());
    painter->restore();
}

QRectF PYInfoPad::drawHeader(QPainter *painter, qreal initY)
{
    QPointF HeaderTopLeft(0,initY);
    QPointF HeaderBottomRight(width(),initY + SIW_HEADER_HEIGHT);
    QRectF HeaderRect(HeaderTopLeft,HeaderBottomRight);

    painter->setPen(Qt::NoPen);
    SIW_APPLY_HEADER_GRADIENT(HeaderRect);
    painter->drawRect(HeaderRect);

    return HeaderRect;
}

bool PYInfoPad::drawCheckBox(QPainter *painter, const QRectF &HeaderRect, qreal initY, int CateIndex)
{
    // draw check box
    QPointF HeaderCheckBoxTopLeft(SIW_LEFT_SPACE,HeaderRect.center().y() - (qreal)SIW_BOX_WIDTH/2);
    QPointF HeaderCheckBoxBottomRight(SIW_LEFT_SPACE + SIW_BOX_WIDTH,HeaderRect.center().y() + (qreal)SIW_BOX_WIDTH/2);
    QRectF HeaderCheckBox(HeaderCheckBoxTopLeft,HeaderCheckBoxBottomRight);
    m_CategoryInfoVec.at(CateIndex)->SetRect(HeaderCheckBox);

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing,false);
    painter->setPen(SIW_BOX_PEN_COLOR);
    QLinearGradient BoxGradient(HeaderCheckBox.topLeft(),HeaderCheckBox.bottomLeft());
    BoxGradient.setColorAt(0.0,SIW_BOX_START_COLOR);
    BoxGradient.setColorAt(1.0,SIW_BOX_END_COLOR);
    painter->setBrush(BoxGradient);
    painter->drawRect(HeaderCheckBox);
    painter->restore();

    qreal textStartX = HeaderCheckBoxBottomRight.x();
    // if icon is visible the draw category icon
    if( m_bIconVisible &&
        m_CategoryInfoVec.at(CateIndex)->IsIconVisible() &&
        !m_CategoryInfoVec.at(CateIndex)->GetIcon().isNull() )
    {
        textStartX += SIW_EXTRA_SPACE * 2 + SIW_ICON_SIZE.width();

        QPointF iconTopLeft(HeaderCheckBoxBottomRight.x() + SIW_EXTRA_SPACE, initY + SIW_EXTRA_SPACE);
        QPixmap pixmap = m_CategoryInfoVec.at(CateIndex)->GetIcon().pixmap(SIW_ICON_SIZE);
        painter->drawPixmap(iconTopLeft,pixmap);
    }

    // draw catogory text
    QPointF CategoryTextPot(textStartX + SIW_BOX_TEXT_SPACE,HeaderCheckBoxBottomRight.y());
    painter->setPen(SIW_CATEGORY_COLOR);
    painter->save();
    QFont CategoryFont;
    CategoryFont.setBold(true);
    painter->setFont(CategoryFont);
    painter->drawText(CategoryTextPot,m_CategoryInfoVec.at(CateIndex)->GetCaption());
    painter->restore();


    // draw horizontal handle
    QPointF CheckBoxLeftPot(HeaderCheckBox.topLeft().x() + SIW_BOX_WIDTH_EXTRA,HeaderCheckBox.center().y());
    QPointF CheckBoxRightPot(HeaderCheckBox.topRight().x() - SIW_BOX_WIDTH_EXTRA,HeaderCheckBox.center().y());
    painter->drawLine(CheckBoxLeftPot,CheckBoxRightPot);

    // draw vertical handle
    if(!m_CategoryInfoVec.at(CateIndex)->IsExpand() || m_CategoryInfoVec.at(CateIndex)->GetData().count() == 0)
    {
        QPointF CheckBoxTopPot(HeaderCheckBox.center().x(),qreal(HeaderCheckBox.center().y() - SIW_BOX_HALF_WIDTH));
        QPointF CheckBoxBottomPot(HeaderCheckBox.center().x(),qreal(HeaderCheckBox.center().y() + SIW_BOX_HALF_WIDTH + 2));
        painter->drawLine(CheckBoxTopPot,CheckBoxBottomPot);
        return false;
    }
    return true;
}

void PYInfoPad::drawSubItems(QPainter *painter, qreal &initY, int CateIndex)
{
    int ItemCnt = m_CategoryInfoVec.at(CateIndex)->GetData().count();

    painter->save();
    painter->setPen(SIW_CONTENT_FRAME_COLOR);
    painter->setRenderHints(QPainter::Antialiasing|QPainter::HighQualityAntialiasing);
    QPointF UnderRectTopLeft(0,initY);
    QPointF UnderRectBottomRight(width(),initY + SIW_ITEM_HEIGHT * ItemCnt);
    QRectF UnderRect(UnderRectTopLeft,UnderRectBottomRight);
    QLinearGradient UnderRectGradient(UnderRect.topLeft(),UnderRect.bottomLeft());
    UnderRectGradient.setColorAt(0.0,SIW_CONTENT_START_COLOR);
    UnderRectGradient.setColorAt(1.0,SIW_CONTENT_END_COLOR);
    painter->setBrush(UnderRectGradient);
    painter->drawRect(UnderRect);
    painter->restore();

    //
    for(int ItemIndex = 0;ItemIndex < ItemCnt; ItemIndex++)
    {
        QPair<QString,QString> OnePair = m_CategoryInfoVec.at(CateIndex)->GetData().at(ItemIndex);

        // draw prompt
        painter->save();
        QFont PromptFont;
        PromptFont.setBold(true);
        painter->setFont(PromptFont);
        QPointF PromptTopLeft(0,initY);
        QPointF PromptBottomRight(width()/2,initY + SIW_ITEM_HEIGHT);
        QRectF PromptRect(PromptTopLeft,PromptBottomRight);
        painter->setPen(SIW_PROMPT_PEN_COLOR);
        painter->drawText(PromptRect,OnePair.first,SIW_PROMPT_ALIGNMENT);
        painter->restore();

        // draw value
        QPointF ValueTopLeft(width()/2,initY);
        QPointF ValueBottomRight(width(),initY + SIW_ITEM_HEIGHT);
        QRectF ValueRect(ValueTopLeft,ValueBottomRight);
        painter->setPen(SIW_VALUE_PEN_COLOR);
        painter->drawText(ValueRect,OnePair.second,SIW_VALUE_ALIGNMENT);

        /// increment initY
        initY += SIW_ITEM_HEIGHT;
    }

}

void PYInfoPad::drawCategory(QPainter *painter)
{
    painter->save();

    int CateCnt =  m_CategoryInfoVec.count();
    qreal initY = 0;

    for(int CateIndex = 0;CateIndex < CateCnt ; CateIndex++)
    {
        // draw header
        QRectF HeaderRect = drawHeader(painter,initY);

        /// increment initY
        initY += SIW_HEADER_HEIGHT;

        if(!drawCheckBox(painter, HeaderRect, (initY - SIW_HEADER_HEIGHT), CateIndex))
        {
            continue;
        }

        drawSubItems(painter, initY, CateIndex);

    }

    m_height = initY ;

    setMinimumHeight(m_height);
    painter->restore();
}

void PYInfoPad::updateAllItemsVisibility(bool visible)
{
    for(int i = 0; i < m_CategoryInfoVec.size(); i++)
    {
        m_CategoryInfoVec.at(i)->SetIconVisible(visible);
    }
    update();
}

/*!
  public interfaces
*/
void PYInfoPad::AddCategory(const QString &strCaption)
{
    CategoryInfo* newCategoryInfo = new CategoryInfo(strCaption);
    newCategoryInfo->SetIcon(QIcon(":/images/info.png"));
    m_CategoryInfoVec.push_back(newCategoryInfo);
}

void PYInfoPad::AddItem(int CategoryIndex,QPair<QString, QString> ItemPair)
{
    if(CategoryIndex >= m_CategoryInfoVec.count())
    {
        return ;
    }

    m_CategoryInfoVec[CategoryIndex]->AddItem(ItemPair);
}

void PYInfoPad::Clear()
{
    int CategoryCnt = m_CategoryInfoVec.count();
    for(int CategoryIndex = 0;CategoryIndex < CategoryCnt;  CategoryIndex++)
    {
        m_CategoryInfoVec[CategoryIndex]->Clear();
    }

    qDeleteAll(m_CategoryInfoVec);
    m_CategoryInfoVec.clear();
}

void PYInfoPad::SetIconVisible(bool visible)
{
    m_bIconVisible = visible;
    updateAllItemsVisibility(visible);

}

bool PYInfoPad::IsIconVisible() const
{
    return m_bIconVisible;
}

void PYInfoPad::SetCategoryIcon(int CategoryIndex, const QIcon &icon)
{
    if(m_CategoryInfoVec.isEmpty())
    {
        return;
    }

    if( 0 <= CategoryIndex && CategoryIndex < m_CategoryInfoVec.size())
    {
        m_CategoryInfoVec[CategoryIndex]->SetIcon(icon);
        update();
    }
}

QIcon PYInfoPad::GetCategoryIcon(int CategoryIndex) const
{
    if(m_CategoryInfoVec.isEmpty())
    {
        return QIcon();
    }

    if( 0 <= CategoryIndex && CategoryIndex < m_CategoryInfoVec.size())
    {
        return m_CategoryInfoVec[CategoryIndex]->GetIcon();
    }

}

void PYInfoPad::Render()
{
    update();
}

int PYInfoPad::CategoryCount() const
{
    m_CategoryInfoVec.size();
}
