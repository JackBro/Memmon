#include "itemwidgetfactory.h"
#include "datachart.h"
#include "../mmutil.h"
#include "../mmdef.h"
#include "../util.h"

#include <QVBoxLayout>
#include <QProcess>
#include <QFile>
#include <QToolTip>
#include <QMessageBox>
#include <QTime>

QStringList ItemWidgetFactory::TextList = QStringList() << "Caption" << "ParentProcessId"
                                                        << "ElapsedTime" << "HandleCount"
                                                        << "ProcessId" ;
QStringList ItemWidgetFactory::ProgressList = QStringList() << "PercentProcessorTime" << "PercentProcessorTime" << "PercentProcessorTime";

QStringList ItemWidgetFactory::BytesList = QStringList() << "MaximumWorkingSetSize" << "MaximumWorkingSetSize"
                                                         << "PeakVirtualSize" << "PeakWorkingSetSize" << "VirtualSize"
                                                         << "WorkingSetSize" << "MinimumWorkingSetSize" << "WorkingSet"
                                                         << "VirtualBytes" << "VirtualBytesPeak" << "WorkingSetPeak"
                                                         << "WorkingSetPrivate";

QStringList ItemWidgetFactory::PathList = QStringList() << "ExecutablePath";


QStringList ItemWidgetFactory::IconList = QStringList() << "Name";

QStringList ItemWidgetFactory::TimeList = QStringList() << "ElapsedTime" << "KernelModeTime" << "UserModeTime";

QStringList ItemWidgetFactory::NumberList = QStringList() << "HandleCount" << "ThreadCount" << "OtherOperationCount"
                                                          << "OtherTransferCount" << "PrivatePageCount" << "ReadOperationCount"
                                                          << "ReadTransferCount" << "WriteOperationCount" << "WriteTransferCount"
                                                          << "PageFaults";

ItemWidgetFactory::ItemWidgetFactory()
{

}


BaseDisplayWidget* ItemWidgetFactory::makeWidget(WidgetType type, QWidget *parent)
{
    switch(type)
    {
        case Text:
        {
            TextDisplayWidget* textWidget = new TextDisplayWidget(parent);
            return textWidget;
        }
        case Bytes:
        {
            BytesDisplayWidget* bytesWidget = new BytesDisplayWidget(parent);
            return bytesWidget;
        }
        case Progress:
        {
            ProgressDisplayWidget* progressWidget = new ProgressDisplayWidget(parent);
            return progressWidget;
        }
        case Icon:
        {
            IconDisplayWidget* iconWidget = new IconDisplayWidget(parent);
            return iconWidget;
        }
        case Time:
        {
            TimeDisplayWidget* timeWidget = new TimeDisplayWidget(parent);
            return timeWidget;
        }
        case Number:
        {
            NumberDisplayWidget* numberWidget = new NumberDisplayWidget(parent);
            return numberWidget;
        }
    }
}

BaseDisplayWidget* ItemWidgetFactory::makeWidgetByName(const QString &strColumnName,QWidget* parent)
{
    if(ProgressList.contains(strColumnName))
    {
        return new ProgressDisplayWidget(parent);
    }
    else if(BytesList.contains(strColumnName) || strColumnName.contains("Bytes"))
    {
        return new BytesDisplayWidget(parent);
    }
    else if(PathList.contains(strColumnName,Qt::CaseInsensitive))
    {
        return new PathDisplayWidget(parent);
    }
    else if(IconList.contains(strColumnName,Qt::CaseInsensitive))
    {
        return new IconDisplayWidget(parent);
    }
    else if(TimeList.contains(strColumnName,Qt::CaseInsensitive))
    {
        return new TimeDisplayWidget(parent);
    }
    else if(NumberList.contains(strColumnName,Qt::CaseInsensitive))
    {
        return new NumberDisplayWidget(parent);
    }
    else
    {
        return new TextDisplayWidget(parent);
    }
}

/***********************************************/
/*! BaseDisplayWidget                          */
/***********************************************/
BaseDisplayWidget::BaseDisplayWidget(QWidget *parent):QWidget(parent)
{

}

void BaseDisplayWidget::setValue(const QString &value)
{
    _value = value;
}

QString BaseDisplayWidget::value() const
{
    return _value;
}

WidgetType BaseDisplayWidget::widgetType() const
{
    return Text;
}
/***********************************************/
/*! TextDisplayWidget                          */
/***********************************************/
TextDisplayWidget::TextDisplayWidget(QWidget *parent):BaseDisplayWidget(parent)
{
    setMouseTracking(true);

    _label = new QLabel(this);
    _label->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(_label);
    setLayout(mainLayout);
}

void TextDisplayWidget::setValue(const QString &value)
{
    if(value.isEmpty())
    {
        _label->setText("N/A");
    }
    else
    {
        _label->setText(value);
        setToolTip(value);
    }
}

QString TextDisplayWidget::value() const
{
    return _label->text();
}

WidgetType TextDisplayWidget::widgetType() const
{
    return Text;
}

QString TextDisplayWidget::text() const
{
    return _label->text();
}

bool TextDisplayWidget::event(QEvent *e)
{
    if(e->type() == QEvent::ToolTip)
    {
        qreal textLength = fontMetrics().width(_label->text());
        if(textLength > this->width())
        {
            return QWidget::event(e);
        }
        else
        {
            return true;
        }
    }
    return QWidget::event(e);
}

/***********************************************/
/*! IconDisplayWidget                          */
/***********************************************/
IconDisplayWidget::IconDisplayWidget(QWidget *parent):BaseDisplayWidget(parent)
{
    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(2);

    _iconLabel = new QLabel(this);
    _label = new QLabel(this);

    mainLayout->addWidget(_iconLabel);
    mainLayout->addWidget(_label);
    mainLayout->addStretch();

    setLayout(mainLayout);
}

void IconDisplayWidget::setValue(const QString &value)
{
    _label->setText(value);
}

QString IconDisplayWidget::value() const
{
    return _label->text();
}

WidgetType IconDisplayWidget::widgetType() const
{
    return Icon;
}

void IconDisplayWidget::setIcon(const QIcon &icon)
{
    _icon = icon;
    _iconLabel->setPixmap(icon.pixmap(QSize(MM::Constant::RowHeight - 4,MM::Constant::RowHeight - 4)));
}

QIcon IconDisplayWidget::icon() const
{
    return _icon;
}

QString IconDisplayWidget::text() const
{
    _label->text();
}

/***********************************************/
/*! ProgressWidget                             */
/***********************************************/
ProgressDisplayWidget::ProgressDisplayWidget(QWidget *parent):BaseDisplayWidget(parent)
{
    _pgsBar = new PYProg(this);
    _pgsBar->setAnimated(true);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(_pgsBar);

    setLayout(mainLayout);
}

void ProgressDisplayWidget::setValue(const QString& strValue)
{
    bool ok = false;
    int value = strValue.toInt(&ok);

    if(ok)
    {
        _pgsBar->setValue(value);
    }
}

QString ProgressDisplayWidget::value() const
{
    QString strValue;
    strValue = tr("%1").arg(_pgsBar->value());
    return strValue;
}

WidgetType ProgressDisplayWidget::widgetType() const
{
    return Progress;
}

QString ProgressDisplayWidget::text() const
{
    return tr("%1").arg(_pgsBar->value());
}
/***********************************************/
/*! BytesDisplayWidget                         */
/***********************************************/
BytesDisplayWidget::BytesDisplayWidget(QWidget *parent):BaseDisplayWidget(parent)
{
    _label = new QLabel(this);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(_label);

    setLayout(mainLayout);

    _showPopup = false;
    _enter = false;
}

void BytesDisplayWidget::setValue(const QString &value)
{
    bool ok = false;
    int nValue = value.toInt(&ok);

    if(ok)
    {
        QString strValue = MM_Util::getDecentSize(nValue);
        _label->setText(strValue);

        if(_data.size() > DefaultDataCount)
        {
            _data.pop_front();
        }
        _data.push_back(nValue);

        if(_enter)
        {
            showDataChart();
        }
    }
}

QString BytesDisplayWidget::value() const
{
    return _label->text();
}

WidgetType BytesDisplayWidget::widgetType() const
{
    return Bytes;
}

QString BytesDisplayWidget::text() const
{
    QString strText;
    for(int i = 0; i < _data.size(); i++)
    {
        strText += tr("%1 ").arg(_data.at(i));
    }
    return strText;
}

void BytesDisplayWidget::clear()
{
    _data.clear();
    update();
}

void BytesDisplayWidget::showPopup(bool show)
{
    _showPopup = show;
}

bool BytesDisplayWidget::isPopupShown() const
{
    return _showPopup;
}

void BytesDisplayWidget::enterEvent(QEvent *e)
{
    QWidget::enterEvent(e);
    showDataChart();
    _enter = true;
}

void BytesDisplayWidget::leaveEvent(QEvent *e)
{
    QWidget::leaveEvent(e);
    _enter = false;
}

void BytesDisplayWidget::showDataChart()
{
    if(_showPopup)
    {
        QPoint globalTopLeft = mapToGlobal(rect().topLeft());
        QRect globalRect(globalTopLeft,QPoint(globalTopLeft.x() + width(),globalTopLeft.y() + height()));

        QString strCaption;
        strCaption = tr("%1:%2").arg(_processName).arg(_columnName);
        DataChart::instance()->setCaption(strCaption);
        DataChart::instance()->setData(_data);

        if(DataChart::instance()->isVisible())
        {
            return;
        }
        else
        {
            DataChart::instance()->showAt(globalRect.center());
        }
    }
}

/***********************************************/
/*! PathDisplayWidget                         */
/***********************************************/

PathDisplayWidget::PathDisplayWidget(QWidget *parent):BaseDisplayWidget(parent)
{
    _button = new QToolButton(this);
    _button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    _button->setIcon(QIcon(":/images/open.png"));
    _button->setAutoRaise(true);
    _button->setAutoRepeat(true);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(_button);

    setLayout(mainLayout);

    connect(_button,SIGNAL(clicked()),this,SLOT(slot_showPath()));
}

void PathDisplayWidget::setValue(const QString &value)
{
    _button->setText(value.isEmpty() ? "N/A" : value);
    _button->setToolTip(value);
}

QString PathDisplayWidget::value() const
{
    _button->text();
}

WidgetType PathDisplayWidget::widgetType() const
{
    return Path;
}

QString PathDisplayWidget::text() const
{
    return _button->text();
}

void PathDisplayWidget::slot_showPath()
{
    QString path(_button->text());

    if(!QFile::exists(path))
    {
        return;
    }
    Util::Shell::LocateFile(path);
}

/***********************************************/
/*! TimeDisplayWidget                         */
/***********************************************/
TimeDisplayWidget::TimeDisplayWidget(QWidget *parent):BaseDisplayWidget(parent)
{
    _lcd = new QLCDNumber(this);
    _lcd->setFrameShape(QFrame::NoFrame);
    _lcd->setSegmentStyle(QLCDNumber::Filled);
    _lcd->setStyleSheet("color:black;");

    QString strTimeFormat("hh:mm:ss.zzz");
    _lcd->setDigitCount(strTimeFormat.length());

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(_lcd);
    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);
}

void TimeDisplayWidget::setValue(const QString &value)
{
    _value = value;
    displayTime();
}

QString TimeDisplayWidget::value() const
{
    return _value;
}

WidgetType TimeDisplayWidget::widgetType() const
{
    return Time;
}

QString TimeDisplayWidget::text() const
{
    return _value;
}

void TimeDisplayWidget::displayTime()
{
    int msecs = _value.toInt();
    QTime time;
    QTime convertedTime = time.addMSecs(msecs);
    QString strTime = convertedTime.toString("hh:mm:ss.zzz");
    _lcd->display(strTime);
}

/***********************************************/
/*! NumberDisplayWidget                        */
/***********************************************/
NumberDisplayWidget::NumberDisplayWidget(QWidget *parent):BaseDisplayWidget(parent)
{
    _label = new QLabel(this);
    _label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(_label);
    setLayout(layout);
}

void NumberDisplayWidget::setValue(const QString &value)
{
    int number = value.toInt();
    QString strNum;
    strNum = tr("%L1").arg(number);
    _label->setText(strNum);
}

QString NumberDisplayWidget::value() const
{
    return _label->text();
}

WidgetType NumberDisplayWidget::widgetType() const
{
    return Number;
}

QString NumberDisplayWidget::text() const
{
    return _label->text();
}


