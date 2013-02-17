#include "xheader.h"

#include <QEvent>
#include <QCoreApplication>
#include <QDebug>

/******************************************************/
/*************| XHeader             |****************/
/******************************************************/
XHeader::XHeader(QWidget *parent):QWidget(parent)
{
    initVars();
    initSettings();
}

/*!
 * private utility functions
 */

void XHeader::initVars()
{
    _width = width();
    _maxWidth = width();
    _mousePressed = false;
    _startDragging = false;
    _isAutoAdjust = false;
    _selectedXPos = 0;
}

void XHeader::initSettings()
{
    setFixedHeight(XPT::Constant::Header_Height);
    setMouseTracking(true);
//    setMinimumWidth(1000);
}

void XHeader::createItems()
{
    qDeleteAll(_headerItems);
    _headerItems.clear();

    qreal itemWidth = (qreal)sizeHint().width()/_labels.size();
    qreal initX = 0;

    ContainerType<qreal> allColumnWidth;
    for(int i = 0;i < _labels.size();i++)
    {
        QPointF topLeft(initX,0);
        QPointF bottomRight(initX + itemWidth,height());
        QRectF itemRect(topLeft,bottomRight);

        QPointF dragTopLeft(initX + itemWidth - XPT::Constant::Header_ExtraSpace,0);
        QPointF dragBottomRight(initX + itemWidth + XPT::Constant::Header_ExtraSpace,height());
        QRectF dragRect(dragTopLeft,dragBottomRight);

        HeaderItem* item = new HeaderItem(_labels.at(i),itemWidth,false);
        item->_thisRect = itemRect;
        item->_separatorXPos = initX + itemWidth;
        item->_dragRect = dragRect;
        item->_startXPos = initX;
        item->_index = i;

        allColumnWidth.push_back(itemWidth);
        _headerItems.push_back(item);

        initX += itemWidth;
    }

    emit columnWidthChanged(allColumnWidth);
}

void XHeader::clearHover()
{
    for(int i = 0;i < _headerItems.size();i++)
    {
        HeaderItem* item = _headerItems[i];
        item->_hover = false;
    }
    update();
}

void XHeader::mousePressEventHandler(QMouseEvent *e)
{
    if(_isAutoAdjust)
    {
        return;
    }

    _mousePressed = true;

#if 1
    for(int i = 0;i < _headerItems.size();i++)
    {
        HeaderItem* item = _headerItems.at(i);
        if(item->_dragRect.contains(e->pos()))
        {
            _startDragging = true;
            _selectedXPos = item->_separatorXPos;
            _selectedItem = item;
            _selectedItem->_index = i;
            _pressedPoint = e->pos();
            setCursor(QCursor(Qt::SplitHCursor));
            return;
        }
    }

    _startDragging = false;
    setCursor(QCursor(Qt::ArrowCursor));
#endif

#if 0
    for(int i = 0;i < _headerItems.size();i++)
    {
        HeaderItem* item = _headerItems[i];
        if(item->_thisRect.contains(e->pos()))
        {
            item->_pressed= true;
        }
        else
        {
            item->_pressed = false;
        }
    }
    update();
#endif

}

void XHeader::mouseReleaseEventHandler(QMouseEvent *e)
{
    if(_isAutoAdjust)
    {
        return;
    }

    if(_startDragging)
    {
        qreal textLength = fontMetrics().width(_selectedItem->_text);
        if((e->pos().x() - _selectedItem->_startXPos) < (2 * textLength))
        {
            _selectedItem->_width = 2 * textLength;
            _selectedItem->_separatorXPos = _selectedItem->_startXPos + _selectedItem->_width;
            _selectedItem->_thisRect = QRectF(QPointF(_selectedItem->_startXPos,0),QPointF(_selectedItem->_separatorXPos,height()));
        }
        else
        {
            _selectedItem->_width = e->pos().x() - _selectedItem->_startXPos;
            _selectedItem->_separatorXPos = e->pos().x();
            _selectedItem->_thisRect = QRectF(QPointF(_selectedItem->_startXPos,0),QPointF(e->pos().x(),height()));
        }

        /// NOTE
        //        resetLatterItems(_selectedItem->_index);
        for(int startIndex = _selectedItem->_index + 1;startIndex < _headerItems.size();startIndex++)
        {

            HeaderItem* item = _headerItems.at(startIndex);
            HeaderItem* prevItem = _headerItems.at(startIndex - 1);

            item->_startXPos = prevItem->_separatorXPos;
            item->_separatorXPos = item->_startXPos + item->_width;
            item->_dragRect = QRectF(QPointF(item->_separatorXPos - XPT::Constant::Header_ExtraSpace,0),QPointF(item->_separatorXPos + XPT::Constant::Header_ExtraSpace,height()));
            item->_thisRect = QRectF(QPointF(prevItem->_separatorXPos,0),QPointF(item->_separatorXPos,height()));
        }

        _selectedItem->_hover = false;
        _selectedItem->_pressed = false;
        _selectedItem->_dragRect = QRectF(QPointF(_selectedItem->_separatorXPos - XPT::Constant::Header_ExtraSpace,0),QPointF(_selectedItem->_separatorXPos + XPT::Constant::Header_ExtraSpace,height()));

        _startDragging = false;

    }
    update();
    setCursor(QCursor(Qt::ArrowCursor));

    updateColumnWidth();
}

qreal XHeader::resetLatterItems(int startIndex)
{
    qreal latterTotalWidth = 0;
    qreal itemWidth = 0;

    int startMoveIndex = startIndex + 1;
    if(startMoveIndex >= _headerItems.size())
    {
        startMoveIndex = _headerItems.size() - 1;
    }

    for(;startMoveIndex < _headerItems.size();startMoveIndex++)
    {

        HeaderItem* item = _headerItems.at(startMoveIndex);
        HeaderItem* prevItem = _headerItems.at(startMoveIndex - 1);

        item->_startXPos = prevItem->_separatorXPos;
        item->_separatorXPos = item->_startXPos + item->_width;
        item->_dragRect = QRectF(QPointF(item->_separatorXPos - XPT::Constant::Header_ExtraSpace,0),QPointF(item->_separatorXPos + XPT::Constant::Header_ExtraSpace,height()));
        item->_thisRect = QRectF(QPointF(prevItem->_separatorXPos,0),QPointF(item->_separatorXPos,height()));
        itemWidth = item->_width;
        latterTotalWidth += itemWidth;
    }
    return latterTotalWidth;
}

void XHeader::updateColumnWidth()
{
    if(!_mousePressed)
    {
        return ;
    }

    _mousePressed = false;

    ContainerType<qreal> allWidth;
    qreal totalWidth = 0;
    qreal itemWidth = 0;
    for(int i = 0;i < _headerItems.size(); i++)
    {
        itemWidth = _headerItems.at(i)->_width;
        allWidth.push_back(itemWidth);
        totalWidth += itemWidth;
    }

    if(_maxWidth < totalWidth)
    {
        _maxWidth = getTotalWidth();
        setMinimumWidth(_maxWidth);
    }
    emit columnWidthChanged(allWidth);
}

bool XHeader::isDraggingItem(QMouseEvent* e)
{

    bool inDragRect = false;
    for(int i = 0;i < _headerItems.size();i++)
    {
        HeaderItem* item = _headerItems[i];
        if(item->_thisRect.contains(e->pos()))
        {
            item->_hover = true;
        }
        else
        {
            item->_hover = false;
        }

        if(item->_dragRect.contains(e->pos()))
        {
            inDragRect = true;
        }
    }

    if(inDragRect)
    {
        setCursor(QCursor(Qt::SplitHCursor));
    }

    return inDragRect;
}

void XHeader::setItemCount(int count)
{
    _itemCount = count;
}

int XHeader::itemCount() const
{
    return _itemCount;
}

int XHeader::getTotalWidth()
{
    int totalWidth = 0;
    for(int i = 0; i < _headerItems.size(); i++)
    {
        totalWidth += _headerItems[i]->_width;
    }
    return totalWidth;
}

void XHeader::mouseMoveEventHandler(QMouseEvent *e)
{
    if(_isAutoAdjust)
    {
        return;
    }

    if(_startDragging)
    {
        int selectedIndex = _selectedItem->_index;

        qreal textLenght = fontMetrics().width(_selectedItem->_text);
        if((_selectedItem->_startXPos + textLenght * 3) > e->pos().x())
        {
            _selectedItem->_width = textLenght * 3;
            _selectedItem->_separatorXPos = _selectedItem->_startXPos + _selectedItem->_width;

            e->accept();
            return;
        }

        ContainerType<qreal> allWidth;
        qreal totalWidth = 0;

        // get the items before the selected items
        for(int i = 0;i < _selectedItem->_index; i++)
        {
            allWidth.push_back(_headerItems.at(i)->_width);
            totalWidth += _headerItems.at(i)->_width;
        }

        // get the selecte item's width
        qreal prevTotalWidth = 0;
        qreal itemWidth = 0;
        for(int startIndex = 0;startIndex < selectedIndex; startIndex++)
        {
            itemWidth = _headerItems.at(startIndex)->_width;
            prevTotalWidth += itemWidth;
        }

        // update selected item's value
        _selectedItem->_separatorXPos = e->pos().x();
        _selectedItem->_width = e->pos().x() - _selectedItem->_startXPos;
        _selectedItem->_dragRect = QRectF(QPointF(e->pos().x() - XPT::Constant::Header_ExtraSpace,0),QPointF(e->pos().x() + XPT::Constant::Header_ExtraSpace,height()));
        _selectedItem->_thisRect = QRectF(QPointF(_selectedItem->_startXPos,0),QPointF(e->pos().x(),height()));

        allWidth.push_back(_selectedItem->_width);
        totalWidth += _selectedItem->_width;

        /// NOTE
        //        resetLatterItems(_selectedItem->_index);
        for(int startIndex = _selectedItem->_index + 1;startIndex < _headerItems.size();startIndex++)
        {

            HeaderItem* item = _headerItems.at(startIndex);
            HeaderItem* prevItem = _headerItems.at(startIndex - 1);

            item->_startXPos = prevItem->_separatorXPos;
            item->_separatorXPos = item->_startXPos + item->_width;
            item->_dragRect = QRectF(QPointF(item->_separatorXPos - XPT::Constant::Header_ExtraSpace,0),QPointF(item->_separatorXPos + XPT::Constant::Header_ExtraSpace,height()));
            item->_thisRect = QRectF(QPointF(prevItem->_separatorXPos,0),QPointF(item->_separatorXPos,height()));

        }


        // get the items after the selected items
        for(int i = _selectedItem->_index + 1;i < _headerItems.size(); i++)
        {
            allWidth.push_back(_headerItems.at(i)->_width);
            totalWidth += _headerItems.at(i)->_width;
        }



        // calculate the latter width
        resetLatterItems(selectedIndex);

        // calculate the total width and resize the width of the widget
//        _maxWidth = (_maxWidth < totalWidth) ? totalWidth : _maxWidth;
        _maxWidth = getTotalWidth();
        setMinimumWidth(_maxWidth);

        // for performance consideration
        // since the performance bottle neck has been resolved , so we just make this functionality open
        //        if(itemCount() <= XPT::Constant::Real_MaxRowCountForMouseMoveUpdate)
        //        {
        emit columnWidthChanged(allWidth);
        //        }
    }
    else if(!_mousePressed && !isDraggingItem(e))
    {
        setCursor(QCursor(Qt::ArrowCursor));
    }

    update();

}

/*!
 * painting functions
 */
void XHeader::drawBackground(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::NoPen);

    qreal vSep = height() * (0.4);

    QRectF topRect(QPointF(0,0),QPointF(width(),vSep));
    QLinearGradient topGradient(topRect.topLeft(),topRect.bottomLeft());
    topGradient.setColorAt(0.0,XPT::Color::Header_StartBackground);
    topGradient.setColorAt(1.0,XPT::Color::Header_StopBackground);
    painter->setBrush(topGradient);
    painter->drawRect(topRect);

    QRectF bottomRect(QPointF(0,vSep),QPointF(width(),height()));
    painter->setBrush(XPT::Color::Header_StopBackground);
    painter->drawRect(bottomRect);

    painter->restore();
}

void XHeader::drawItems(QPainter *painter)
{
    painter->save();

    qreal initX = 0;
    for(int itemIndex = 0;itemIndex < _headerItems.size();itemIndex++)
    {
        HeaderItem* item = _headerItems.at(itemIndex);

        if(item->_hover)
        {
            drawHoverItem(painter,item,initX);
        }
        else if(item->_pressed)
        {
            drawPressedItem(painter,item,initX);
        }
        else
        {
            drawNormalItem(painter,item,initX);
        }
    }

    painter->restore();
}

void XHeader::drawSeparators(QPainter *painter)
{
    painter->save();
    painter->setPen(XPT::Color::Header_Separator);
    qreal initX = 0;

    for(int i = 0;i < _headerItems.size(); i++)
    {
        initX += _headerItems.at(i)->_width;

        QPointF topPoint(initX,0);
        QPointF bottomPoint(initX,height());
        painter->drawLine(topPoint,bottomPoint);
    }

    painter->restore();
}

void XHeader::drawHoverItem(QPainter *painter, HeaderItem *item, qreal& initX)
{
    painter->save();

    QLinearGradient itemGradient(item->_thisRect.topLeft(),item->_thisRect.bottomLeft());
    itemGradient.setColorAt(0.0,XPT::Color::Header_HoverStartBackground);
    itemGradient.setColorAt(0.5,XPT::Color::Header_HoverStartBackground);
    itemGradient.setColorAt(1.0,XPT::Color::Header_HoverStopBackground);
    painter->setPen(Qt::NoPen);
    painter->setBrush(itemGradient);
    painter->setOpacity(XPT::Constant::Header_HoverOpacity);
    painter->drawRect(item->_thisRect);

    painter->restore();

    painter->save();

    QFont boldFont;
    boldFont.setBold(true);
    //    boldFont.setPointSize(XPT::Constant::Header_Height/2);

    painter->setFont(boldFont);

    painter->setPen(XPT::Color::Header_HoverTitle);
    painter->drawText(item->_thisRect,item->_text,Qt::AlignVCenter|Qt::AlignHCenter);

    painter->restore();

    initX += item->_width;
}

void XHeader::drawPressedItem(QPainter *painter, HeaderItem *item, qreal& initX)
{
    painter->save();

    QLinearGradient itemGradient(item->_thisRect.topLeft(),item->_thisRect.bottomLeft());
    itemGradient.setColorAt(0.0,XPT::Color::Header_PressedStartBackground);
    itemGradient.setColorAt(0.5,XPT::Color::Header_PressedStartBackground);
    itemGradient.setColorAt(1.0,XPT::Color::Header_PressedStopBackground);
    painter->setPen(QPen(XPT::Color::Header_Separator));
    painter->setBrush(itemGradient);
    painter->drawRect(item->_thisRect);

    painter->setPen(XPT::Color::Header_NormalTitle);
    painter->drawText(item->_thisRect,item->_text,Qt::AlignVCenter|Qt::AlignHCenter);

    painter->restore();

    initX += item->_width;

}

void XHeader::drawNormalItem(QPainter *painter, HeaderItem *item, qreal& initX)
{

    painter->save();
    painter->setPen(Qt::NoPen);

    qreal vSep = height() * (0.4);

    QRectF topRect(QPointF(initX,0),QPointF(width(),vSep));
    QLinearGradient topGradient(topRect.topLeft(),topRect.bottomLeft());
    topGradient.setColorAt(0.0,XPT::Color::Header_StartBackground);
    topGradient.setColorAt(1.0,XPT::Color::Header_StopBackground);
    painter->setBrush(topGradient);
    painter->drawRect(topRect);

    QRectF bottomRect(QPointF(initX,vSep),QPointF(initX + item->_width,height()));
    painter->setBrush(XPT::Color::Header_StopBackground);
    painter->drawRect(bottomRect);


    QFont textFont;
    if(_isAutoAdjust)
    {
        textFont.setBold(true);
        textFont.setFamily("Times New Roman");
    }
    //    textFont.setPointSize(XPT::Constant::Header_Height/2);
    painter->setFont(textFont);

    painter->setPen(XPT::Color::Header_NormalTitle);
    painter->drawText(item->_thisRect,item->_text,Qt::AlignVCenter|Qt::AlignHCenter);

    painter->restore();

    initX += item->_width;

}

/*!
 * reimplemented functions
 */

void XHeader::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setRenderHints(QPainter::Antialiasing,false);
    drawBackground(&painter);
    drawItems(&painter);
    drawSeparators(&painter);

}

void XHeader::mouseMoveEvent(QMouseEvent *e)
{
    mouseMoveEventHandler(e);
}

void XHeader::mousePressEvent(QMouseEvent *e)
{
    mousePressEventHandler(e);
}

void XHeader::mouseReleaseEvent(QMouseEvent *e)
{
    mouseReleaseEventHandler(e);
}

void XHeader::leaveEvent(QEvent *e)
{
    clearHover();
    QWidget::leaveEvent(e);
}

void XHeader::resizeEvent(QResizeEvent *)
{
    if(_isAutoAdjust)
    {
        QVector<qreal> textRatios;
        qreal totalTextLength = 0;

        // get text width ratio
        for(int i = 0; i < _headerItems.size(); i++)
        {
            totalTextLength += fontMetrics().width(_headerItems[i]->_text);
        }

        for(int i = 0; i < _headerItems.size(); i++)
        {
            qreal ratio = (qreal)fontMetrics().width(_headerItems[i]->_text)/totalTextLength;
            textRatios.push_back(ratio);
        }

        // calculate geometries of header items
        ContainerType<qreal> allColumnWidth;
        qreal initX = 0;
        for(int i = 0; i < textRatios.size(); i++)
        {
            qreal headerWidth = width() * textRatios.at(i);
            allColumnWidth.push_back(headerWidth);

            HeaderItem* item = _headerItems[i];
            item->_startXPos = initX;
            item->_width = headerWidth;
            item->_separatorXPos = item->_startXPos + headerWidth;
            item->_dragRect = QRectF(QPointF(item->_separatorXPos - XPT::Constant::Header_ExtraSpace,0),QPointF(item->_separatorXPos + XPT::Constant::Header_ExtraSpace,height()));
            item->_thisRect = QRectF(QPointF(item->_startXPos,0),QPointF(item->_separatorXPos,height()));

            initX += headerWidth;
        }

        update();
        // emit columnWidthChanged signal
        emit columnWidthChanged(allColumnWidth);
    }
}

/*!
 * public interfaces
 */

void XHeader::setLabels(const QStringList &labels)
{
    _labels = labels;
    createItems();
    update();
}

QStringList XHeader::labels() const
{
    return _labels;
}

void XHeader::clear()
{
    qDeleteAll(_headerItems);
}

void XHeader::sortByThis(const QStringList &labels)
{
    if(labels.size() != _headerItems.size())
    {
        setLabels(labels);
    }

    for(int colIndex = 0;colIndex < labels.size();colIndex++)
    {
        _headerItems[colIndex]->_text = labels.at(colIndex);
    }
    update();
}

void XHeader::setAutoAdjust(bool adjust)
{
    _isAutoAdjust = adjust;

    if(_isAutoAdjust)
    {
        QEvent e(QEvent::Resize);
        QCoreApplication::sendEvent(this,&e);
    }
    else
    {
        // what if the previous state is auto adjusted
        // then reset to disable auto adjusted state it needs update
        update();
    }
}

bool XHeader::isAutoAdjust() const
{
    return _isAutoAdjust;
}
