#include "xprocesstable.h"
#include "xprocesstable_p.h"
#include "../logutil.h"
#include "../mmutil.h"
#include "../mmdef.h"

#include <QVBoxLayout>
#include <QScrollArea>
#include <QIcon>
#include <QScrollBar>


/*******************************************/
/*! XProcessItem                           */
/*******************************************/
XProcessItem::XProcessItem(const QStringList &columns,int pidColumnIndex,int nameColumnIndex, QWidget *parent):QObject(parent),_parent(parent),
    _pidColumnIndex(pidColumnIndex),_nameColumnIndex(nameColumnIndex)
{
    _expanded = true;
    _icon = QIcon(":/images/process.png");

    createWidgetByColumns(columns);
}

XProcessItem::XProcessItem(QWidget *parent):QObject(parent),_parent(parent)
{
    _expanded = true;
    _icon = QIcon(":/images/process.png");
}

XProcessItem::~XProcessItem()
{
    resetColumns();
    clearChildren();
}

void XProcessItem::setName(const QString& strName)
{
    _name = strName;
}

QString XProcessItem::name() const
{
    return _name;
}

void XProcessItem::setPid(uint32_t pid)
{
    _pid = pid;
}

uint32_t XProcessItem::pid() const
{
    return _pid;
}

void XProcessItem::setRect(const QRectF& rect)
{
    _expandboxRect = rect;
}

QRectF XProcessItem::rect() const
{
    return _expandboxRect;
}

void XProcessItem::setItemRect(const QRectF &rect)
{
    _thisRect = rect;
}

QRectF XProcessItem::itemRect() const
{
    return _thisRect;
}

void XProcessItem::setExpanded(bool expand)
{
    _expanded = expand;
}

bool XProcessItem::expanded() const
{
    return _expanded;
}

void XProcessItem::setIcon(const QIcon& icon)
{
    _icon = icon;
}

QIcon XProcessItem::icon() const
{
    return _icon;
}

void XProcessItem::resetColumns()
{
    qDeleteAll(_widgets);
    _widgets.clear();
}

void XProcessItem::clearChildren()
{
    qDeleteAll(_children);
    _children.clear();
}

void XProcessItem::addChild(XProcessItem *child)
{
    _children.push_back(child);
}

bool XProcessItem::hasChild() const
{
    return !_children.isEmpty();
}

Container<XProcessItem*> XProcessItem::children()
{
    return _children;
}

Container<BaseDisplayWidget*> XProcessItem::getColumnWidgets() const
{
    return _widgets;
}

void XProcessItem::updateInfo(const QStringList &infoList)
{
//    Q_ASSERT(infoList.size() == _widgets.size());
    QStringList newInfoList(infoList);
//    newInfoList.removeAt(_nameColumnIndex);
//    newInfoList.removeAt(_pidColumnIndex);
    if(newInfoList.size() != _widgets.size())
    {
        qWarning("columns and widget size don't match !!!");
        return ;
    }
//    Q_ASSERT(newInfoList.size() == _widgets.size());

    for(int i = 0;i < newInfoList.size();i++)
    {
        BaseDisplayWidget* widget = _widgets[i];
        widget->setValue(newInfoList.at(i));
    }
}

void XProcessItem::createWidgetByColumns(const QStringList &columns)
{
//    QStringList newColumns(columns);

//    int tmpNameColumnIndex = -1;
//    int tmpPidColumnIndex = -1;
//    for(int i = 0;i < columns.size();i++)
//    {
//        if(columns.at(i) == "Name")
//        {
//            tmpNameColumnIndex = i;
//        }
//        else if(columns.at(i) == "IDProcess")
//        {
//            tmpPidColumnIndex = i;
//        }
//    }

//    newColumns.removeAt(tmpPidColumnIndex);
//    newColumns.removeAt(tmpNameColumnIndex);

    for(int i = 0;i < columns.size();i++)
    {
//        if(i == _pidColumnIndex || i == _nameColumnIndex)
//        {
//            continue;
//        }
        BaseDisplayWidget* widget = ItemWidgetFactory::makeWidgetByName(columns.at(i),_parent);
        widget->setProcessName(_name);

        if(widget->widgetType() == Icon)
        {
            IconDisplayWidget* iconWidget = (IconDisplayWidget*)widget;
            iconWidget->setColumnName(columns.at(i));
            iconWidget->setIcon(QIcon(":/images/process.png"));
            _widgets.push_back(iconWidget);
        }
        else
        {
            widget->setColumnName(columns.at(i));
            _widgets.push_back(widget);
        }
    }
}

void XProcessItem::setPidColumnIndex(int index)
{
    _pidColumnIndex = index;
}

int XProcessItem::pidColumnIndex() const
{
    return _pidColumnIndex;
}

void XProcessItem::setNameColumnIndex(int index)
{
    _nameColumnIndex = index;
}

int XProcessItem::nameColumnIndex() const
{
    return _nameColumnIndex;
}

void XProcessItem::clearHistoryData()
{
    for(int i = 0;i < _widgets.size();i++)
    {
        if(_widgets.at(i)->widgetType() == Bytes)
        {
            BytesDisplayWidget* bytesWidget = (BytesDisplayWidget*)_widgets[i];
            bytesWidget->clear();
        }
    }
}

void XProcessItem::showPopup(bool show)
{
    for(int i = 0;i < _widgets.size();i++)
    {
        if(_widgets.at(i)->widgetType() == Bytes)
        {
            BytesDisplayWidget* bytesWidget = (BytesDisplayWidget*)_widgets[i];
            bytesWidget->showPopup(show);
        }
    }
}

QString XProcessItem::contents()
{
    QString strContents;
    for(int i = 0; i < _widgets.size(); i++)
    {
        if(_widgets[i])
        {
            strContents += tr(" %1").arg(_widgets[i]->text());
        }
    }
    return strContents;
}

void XProcessItem::find(const QString &expr)
{
    const int widgetCnt = _widgets.size();
    for(int i = 0; i < widgetCnt; i++)
    {
        _widgets[i]->find(expr);
    }
}

void XProcessItem::setDataCount(int dataCnt)
{
    const int widgetCnt = _widgets.size();
    for(int i = 0;i < widgetCnt; i++)
    {
        if(_widgets[i]->widgetType() == Bytes)
        {
            BytesDisplayWidget* widget = dynamic_cast<BytesDisplayWidget*>(_widgets[i]);
            widget->setDataCount(dataCnt);
        }
    }
}

/*******************************************/
/*! XProcessTable                          */
/*******************************************/

XProcessTable::XProcessTable(QWidget *parent) :
    QWidget(parent)
{

    QVBoxLayout* thisLayout = new QVBoxLayout;
    thisLayout->setContentsMargins(0,0,0,0);
    XScrollArea* wrapArea = new XScrollArea(this);
    wrapArea->setWidgetResizable(true);
    QWidget* mainWidget = new QWidget(this);
    header = new XHeader(this);
    d_ptr = new XProcessTablePrivate(this);

    XScrollArea*  area= new XScrollArea(this);
    area->setWidget(d_ptr);
    area->setWidgetResizable(true);
    area->setFrameShape(QFrame::NoFrame);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);

    mainLayout->addWidget(header);
    mainLayout->addWidget(area);

    mainWidget->setLayout(mainLayout);

    wrapArea->setWidget(mainWidget);
    thisLayout->addWidget(wrapArea);
    setLayout(thisLayout);


    connect(header,SIGNAL(columnWidthChanged(QVector<qreal>)),d_ptr,SLOT(columnWidthChanged(QVector<qreal>)));
    connect(area,SIGNAL(setViewportX(int,int)),d_ptr,SLOT(setViewportX(int,int)));
    connect(area,SIGNAL(setViewportY(int,int)),d_ptr,SLOT(setViewportY(int,int)));

    connect(wrapArea->horizontalScrollBar(),SIGNAL(valueChanged(int)),area,SLOT(layoutVScrollBar(int)));

    initContextMenu();
}

///
/// PUBLIC FUNCTIONS
///
void XProcessTable::setColumns(const QStringList &colList)
{
    header->setLabels(colList);

    // clear existing column widgets and create new widgets
    clear();
}

void XProcessTable::sortByColumns(const QStringList &colList)
{
    header->sortByThis(colList);
}

void XProcessTable::addProcess(XProcessItem *item)
{
    Q_D(XProcessTable);
    d->addItem(item);
    _pids.push_back(item->pid());
}

void XProcessTable::addProcess(uint32_t pid)
{
    _pids.push_back(pid);
    createItemById(pid);
}

void XProcessTable::updateProcessInfo(uint32_t pid,const QStringList& infoList)
{
    if(_pid2ItemMap.contains(pid))
    {
        XProcessItem* item = _pid2ItemMap[pid];
        item->updateInfo(infoList);
    }
    else
    {
        createItemById(pid);
        updateProcessInfo(pid,infoList);
    }
}

void XProcessTable::deleteProcess(uint32_t pid)
{
    if(_pids.contains(pid))
    {
        int index = _pids.indexOf(pid);
        _pids.remove(index);

        // _pid2ItemMap is only a pid to item map
        // you cannot release the item memory by deleting the item
        // from _pid2ItemMap
        _pid2ItemMap.remove(pid);
        d_ptr->removeItemByPid(pid);
    }
}
void XProcessTable::setHorizontalLabels(const QStringList &labels)
{
    header->setLabels(labels);
}

XProcessTablePrivate* XProcessTable::d()
{
    return d_ptr;
}


void XProcessTable::clear()
{
    Q_D(XProcessTable);
    d->clear();
}

void XProcessTable::setAutoAdjust(bool adjust)
{
    header->setAutoAdjust(adjust);
}

bool XProcessTable::isAutoAdjust() const
{
    return header->isAutoAdjust();
}

QByteArray XProcessTable::contents()
{
    Q_D(XProcessTable);
    return d->contents();
}

void XProcessTable::find(const QString &expr)
{
    Q_D(XProcessTable);
    d->find(expr);
}

void XProcessTable::setDataCount(int dataCnt)
{
    Q_D(XProcessTable);
    d->setDataCount(dataCnt);
}

///
/// REIMPL
///
void XProcessTable::contextMenuEvent(QContextMenuEvent *e)
{
    _contextMenu->exec(e->globalPos());
    e->accept();
}

///
/// PRIVATE FUNCTIONS
///
void XProcessTable::initContextMenu()
{
    _contextMenu = new QMenu(this);

    createAction(AutoAdjust,XPT::String::CM_AutoAdjust);

    for(int i = 0; i < ActionCount; i++)
    {
        _contextMenu->addAction(_actions[i]);
    }
}

void XProcessTable::createItemById(uint32_t pid)
{
    XProcessItem* newItem = new XProcessItem(this);
    newItem->setPid(pid);

    _pid2ItemMap.insert(pid,newItem);
}

QAction* XProcessTable::createAction(Actions act, const QString &strText, const QIcon &icon)
{
    QAction* action = new QAction(this);
    action->setText(strText);
    action->setIcon(icon);
    connect(action,SIGNAL(triggered()),this,SLOT(slot_actionHandler()));

    _actions[act] = action;
    return action;
}

///
/// PRIVATE SLOT FUNCTIONS
///
void XProcessTable::slot_actionHandler()
{
    GET_SENDER(QAction);

    if(who == _actions[AutoAdjust])
    {
        header->setAutoAdjust(!header->isAutoAdjust());
    }
}

/*******************************************/
/*! XScrollArea                            */
/*******************************************/

XScrollArea::XScrollArea(QWidget *parent):QScrollArea(parent)
{
    _startX = 0;
    connect(horizontalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(hScrollBarValueChanged(int)));
    emit setViewportX(_startX,width());
}

void XScrollArea::resizeEvent(QResizeEvent *e)
{
    QScrollArea::resizeEvent(e);
    emit setViewportX(_startX,_startX + e->size().width());
}


void XScrollArea::keyPressEvent(QKeyEvent *e)
{
    QScrollArea::keyPressEvent(e);
    emit passKeyPressEvent(e);
}

void XScrollArea::hScrollBarValueChanged(int value)
{
    _startX = value;
    emit setViewportX(_startX,_startX + width());
}
void XScrollArea::vScrollBarValueChanged(int value)
{
    _startY = value;
    emit setViewportY(_startY,_startY + height());
}

void XScrollArea::layoutVScrollBar(int hValue)
{
    QPoint movePoint(width() - verticalScrollBar()->height(),0);
    LOG_VAR(movePoint);

    verticalScrollBar()->move(QPoint(0,0));
}

/*******************************************/
/*! XProcessTablePrivate                   */
/*******************************************/
XProcessTablePrivate::XProcessTablePrivate(XProcessTable *parent):QWidget(parent),q_ptr(parent)
{
    _startX = 0;
    _stopX = 0;
    _startY = 0;
    _stopY = 0;
}

///
/// REIMPLEMENTED FUNCTIONS
///
void XProcessTablePrivate::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    drawBackground(&painter);
    drawHighlightBar(&painter);
    drawColumnLines(&painter);

    // since we don't draw the process names
    // we disable this function for performance optimization
    drawItems(&painter);

    if(_height < height())
    {
        _height = height();
    }
    setFixedHeight(_height);

}


void XProcessTablePrivate::mousePressEvent(QMouseEvent *e)
{
    if(_items.isEmpty())
    {
        return ;
    }
    _pressedPoint = e->pos();

    for(int i = 0;i < _rectItems.size();i++)
    {
        RectItem item = _rectItems.at(i);
        if(item.first.contains(e->pos()))
        {
            bool expand = item.second->expanded() ? false : true;
            item.second->setExpanded(expand);
            break;
        }
    }

    fireClickSignals(e);
    update();
}

void XProcessTablePrivate::resizeEvent(QResizeEvent *e)
{
    relayout();
    e->accept();
    update();
}

///
/// PAINTING FUNCTIONS
///
void XProcessTablePrivate::drawBackground(QPainter *painter)
{
    painter->save();
    painter->setPen(XPT::Color::PenColor);

    int initY = 0;
    int counter = 0;

    while(initY < height())
    {
        if(counter++ % 2 == 0)
        {
            painter->setBrush(XPT::Color::LightColor);
        }
        else
        {
            painter->setBrush(XPT::Color::DarkColor);
        }

        QRect itemRect(QPoint(0,initY),QPoint(width(),initY + XPT::Constant::RowHeight));
        painter->drawRect(itemRect);

        // increment initY
        initY += XPT::Constant::RowHeight;

    }

    painter->restore();
}

void XProcessTablePrivate::drawColumnLines(QPainter *painter)
{
    painter->save();
    painter->setPen(XPT::Color::ColumnLineColor);

    qreal initX = 0;

    for(int index = 0;index < _allColumnWidth.size();index++)
    {
        initX += _allColumnWidth.at(index);

        QPointF topPoint(initX,0);
        QPointF bottomPoint(initX,height());
        painter->drawLine(topPoint,bottomPoint);
    }

    painter->restore();
}

void XProcessTablePrivate::drawHighlightBar(QPainter *painter)
{

    if(_pressedPoint.isNull())
    {
        return ;
    }

    painter->save();
    painter->setOpacity(XPT::Constant::HighlightBarOpacity);
    painter->setPen(XPT::Color::PenColor);
    painter->setBrush(XPT::Color::HighlightColor);

    int startY = _pressedPoint.y()/XPT::Constant::RowHeight;
    QRectF highlightRect(QPointF(0,startY * XPT::Constant::RowHeight),QPointF(width(),(startY + 1) * XPT::Constant::RowHeight));
    painter->drawRect(highlightRect);
    painter->restore();
}


void XProcessTablePrivate::drawItems(QPainter *painter)
{
    _rectItems.clear();

    painter->save();

    qreal initY = 0;
    qreal initX = XPT::Constant::LeftSpace;

    for(int itemIndex = 0;itemIndex < _items.size();itemIndex++)
    {
        initX = XPT::Constant::LeftSpace;
        drawItem(painter,_items.at(itemIndex),initX,initY);
    }

    _height = initY;


    painter->restore();
}

void XProcessTablePrivate::drawItem(QPainter *painter, XProcessItem *item,qreal initX,qreal& initY)
{
    painter->save();

//    qreal initX = XPT::Constant::LeftSpace;

    // draw expandbox if the item has child items
#if 0
    if(item->hasChild())
    {
        QRectF expandboxRect = getExpandBox(initX,initY);
        drawExpandBox(painter,expandboxRect,item);
        drawExpandBoxHandles(painter,expandboxRect,item->expanded());
    }
#endif

    // draw icon
//    if(!item->icon().isNull())
//    {
//        drawIcon(painter,initX,initY,item);
//    }


//    drawText(painter,initX,initY,item);

    initY += XPT::Constant::RowHeight;

#if 0
    if(item->hasChild() && item->expanded())
    {
        initX += XPT::Constant::ExtraSpace * 2 + XPT::Constant::ExpandBoxSize.width() ;
        for(int childIndex = 0;childIndex < item->children().size();childIndex++)
        {
            drawItem(painter,item->children().at(childIndex),initX,initY);
        }
    }
#endif
//    if(item->hasChild())
//    {
////        drawChildItems(painter,item->children());
//    }



    painter->restore();
}

void XProcessTablePrivate::drawChildItems(QPainter *painter,const Container<XProcessItem*>& children)
{
    painter->save();


    painter->restore();
}

void XProcessTablePrivate::relayout()
{
    qreal initY = 0;
    layoutWidgets(_items,initY);
}

void XProcessTablePrivate::layoutWidgets(const Container<XProcessItem*> items,qreal& initY)
{
    qreal initX = 0;

    if(!_allColumnWidth.isEmpty())
    {
        initX = _allColumnWidth.at(0);
    }

    for(int i = 0;i < items.size();i++)
    {
        layoutWidget(items.at(i),initY);
    }

}

void XProcessTablePrivate::layoutWidget(XProcessItem *item,qreal& initY)
{
    // guard clause
    // for safe releasing memory use
    if(!item)
    {
        return ;
    }

    setItemRect(item, initY);

    if(_allColumnWidth.size() > 0)
    {
        qreal initX = 0;
        qreal endX = 0;

        Container<BaseDisplayWidget*> widgets = item->getColumnWidgets();
        for(int i = 0;i < widgets.size();i++)
        {
            QPointF topLeft(initX,initY);
            QPointF bottomRight(initX + _allColumnWidth.at(i),initY + XPT::Constant::RowHeight);

            BaseDisplayWidget* widget = widgets[i];

            if(!widget)
            {
                continue;
            }

            if(widget->widgetType() != Progress)
            {
                topLeft.setX(topLeft.x() + XPT::Constant::ExtraSpace);
                bottomRight.setX(bottomRight.x() - XPT::Constant::ExtraSpace);
            }
            else
            {
                topLeft.setY(topLeft.y() + XPT::Constant::TinySpace);
                bottomRight.setY(bottomRight.y() - XPT::Constant::TinySpace);
            }

            QRectF itemRect(topLeft,bottomRight);

            widget->resize(itemRect.size().toSize());
            widget->move(topLeft.toPoint());

            // hide name since we will draw it
            widget->show();

            // increment initX
            initX += _allColumnWidth.at(i);

            if(item->hasChild() && item->expanded())
            {
                layoutWidgets(item->children(),initY);
            }
        }

        // increment initY
        initY += XPT::Constant::RowHeight;

        setMinimumHeight(initY);
    }
}

QRectF XProcessTablePrivate::getExpandBox(qreal initX,qreal initY)
{
    QPointF topLeft(initX,initY + XPT::Constant::RowHeight/2 - XPT::Constant::ExpandBoxSize.height()/2);
    QPointF bottomRight(initX + XPT::Constant::ExpandBoxSize.width(),initY + XPT::Constant::RowHeight/2 + XPT::Constant::ExpandBoxSize.height()/2);
    QRectF expandBoxRect(topLeft,bottomRight);
    return expandBoxRect;
}

void XProcessTablePrivate::drawExpandBox(QPainter* painter,const QRectF& expandboxRect,XProcessItem* item)
{
    QLinearGradient boxGradient(expandboxRect.topLeft(),expandboxRect.bottomLeft());
    boxGradient.setColorAt(0.0,XPT::Color::ExpandBoxStart);
    boxGradient.setColorAt(1.0,XPT::Color::ExpandBoxStop);

    painter->setPen(XPT::Color::ExpandBoxPen);
    painter->setBrush(boxGradient);

    painter->drawRect(expandboxRect);

    RectItem theItem(expandboxRect,item);
    _rectItems.push_back(theItem);
}

void XProcessTablePrivate::drawExpandBoxHandles(QPainter *painter, const QRectF &expandboxRect, bool expand)
{
    painter->setPen(XPT::Color::ExpandBoxPen);

    // draw horizontal handle
    QPointF hLeftPoint(expandboxRect.topLeft().x() + XPT::Constant::BoxExtraSpace,expandboxRect.center().y());
    QPointF hRightPoint(expandboxRect.topRight().x() - XPT::Constant::BoxExtraSpace,expandboxRect.center().y());
    painter->drawLine(hLeftPoint,hRightPoint);

    // draw vertical handle
    if(!expand)
    {
        QPointF vTopPoint(expandboxRect.center().x(),expandboxRect.topLeft().y() + XPT::Constant::BoxExtraSpace);
        QPointF vBottomPoint(expandboxRect.center().x(),expandboxRect.bottomLeft().y() - XPT::Constant::BoxExtraSpace);
        painter->drawLine(vTopPoint,vBottomPoint);
    }
}

void XProcessTablePrivate::drawIcon(QPainter *painter, qreal initX, qreal initY, XProcessItem *item)
{
    qreal iconX = initX + XPT::Constant::ExtraSpace;

    if(item->hasChild())
    {
        iconX += XPT::Constant::ExpandBoxSize.width();
    }
    else
    {
        iconX += XPT::Constant::BoxExtraSpace;
    }

    QPointF topLeft(iconX + XPT::Constant::IconExtraSpace,initY + XPT::Constant::IconExtraSpace);
    QPointF bottomRight(iconX + XPT::Constant::IconSize.width() - XPT::Constant::IconExtraSpace,initY + XPT::Constant::IconSize.height() - XPT::Constant::IconExtraSpace);
    QRect pixmapRect(topLeft.toPoint(),bottomRight.toPoint());
    painter->drawPixmap(pixmapRect,item->icon().pixmap(pixmapRect.size()));
}

void XProcessTablePrivate::drawText(QPainter *painter, qreal initX, qreal initY, XProcessItem *item)
{
    qreal textX = initX;

    if(item->hasChild())
    {
        textX += 2 * XPT::Constant::ExtraSpace + XPT::Constant::ExpandBoxSize.width();
    }

    if(!item->icon().isNull())
    {
        textX += XPT::Constant::IconSize.width() + XPT::Constant::ExtraSpace * 2;
    }

    qreal availableTextWidth = width();

    if(_allColumnWidth.size() > 0)
    {
        availableTextWidth = _allColumnWidth.size() > 0 ? (_allColumnWidth.at(0) - textX) : _allColumnWidth.at(0);
    }

    QRectF textRect(QPointF(textX,initY),QPointF(textX + availableTextWidth,initY + XPT::Constant::RowHeight));
    painter->setPen(XPT::Color::TextColor);

    QFont textFont;
    textFont.setBold(true);
    painter->setFont(textFont);

    QString strDecentName = getDottedString(item->name(),availableTextWidth);
    painter->drawText(textRect,strDecentName,Qt::AlignLeft|Qt::AlignVCenter);
}

QString XProcessTablePrivate::getDottedString(const QString &originalStr, qreal width)
{
    qreal textLength = fontMetrics().width(originalStr);
    QString str(originalStr);
    if(textLength < width)
    {
        return str;
    }

    for(int i = 0;i < originalStr.length();i++)
    {
        str = tr("%1...").arg(originalStr.left(i));
        if(fontMetrics().width(str) >= width)
        {
            return str;
        }
    }
}

void XProcessTablePrivate::setItemRect(XProcessItem *item, qreal initY)
{
    QPointF topLeft(0, initY);
    QPointF bottomRight(width(), initY + XPT::Constant::RowHeight);
    QRectF rect(topLeft, bottomRight);
    item->setItemRect(rect);
}

void XProcessTablePrivate::fireClickSignals(QMouseEvent *e)
{
    const int proCnt = _items.size();
    for(int i = 0; i < proCnt; i++)
    {
        const XProcessItem* item = _items.at(i);
        if(item && item->itemRect().contains(e->pos()))
        {
            emit q_ptr->sig_processClicked(item->pid());
            emit q_ptr->sig_processClicked(item->name());
        }
    }
}

///
/// PRIVATE SLOT FUNCTIONS
///
void XProcessTablePrivate::columnWidthChanged(QVector<qreal> allColumnWidth)
{

    _allColumnWidth = allColumnWidth;
    relayout();
    update();

}
void XProcessTablePrivate::setViewportX(int x1,int x2)
{
    _startX = x1;
    _stopX = x2;
}

void XProcessTablePrivate::setViewportY(int y1,int y2)
{
    _startY = y1;
    _stopY = y2;
}


///
/// PUBLIC FUNCTIONS
///
void XProcessTablePrivate::addItem(XProcessItem *item)
{
    _items.push_back(item);
    update();
}

void XProcessTablePrivate::clear()
{
    qDeleteAll(_items);
    _items.clear();
}
void XProcessTablePrivate::removeItemByPid(uint32_t pid)
{
    for(int i = 0;i < _items.size();i++)
    {
        // we don't need to release the item memory ,
        // because the item is allocated outside of the table
        // so it will be released by the outside allocator
        XProcessItem* item = _items[i];

        if(item->pid() == pid)
        {
            _items.removeAt(i);
            relayout();
            return;
        }
    }
}

QByteArray XProcessTablePrivate::contents()
{
    QByteArray array;
    QString strContents;
    for(int i = 0; i < _items.size(); i++)
    {
        strContents += tr("%1 \n").arg(_items.at(i)->contents());
    }
    array.append(strContents);
    return array;
}

void XProcessTablePrivate::find(const QString &expr)
{
    const int widgetCnt = _items.size();
    for(int i = 0; i < widgetCnt; i++)
    {
        _items[i]->find(expr);
    }
}

void XProcessTablePrivate::setDataCount(int dataCnt)
{
    const int widgetCnt = _items.size();
    for(int i = 0; i < widgetCnt; i++)
    {
        _items[i]->setDataCount(dataCnt);
    }
}
