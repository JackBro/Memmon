#include "mmuiproxy.h"

MmUiProxy::MmUiProxy(QWidget *parent) :
    QObject(parent),_parent(parent)
{
    initWidgets();
    initLabels();
}

void MmUiProxy::initWidgets()
{
    PYProg* cpuProg = new PYProg(_parent);
    ADD_WIDGET(Widget,Widget_CpuIndicator,cpuProg)

    PYProg* memProg = new PYProg(_parent);
    ADD_WIDGET(Widget,Widget_MemIndicator,memProg)
}

void MmUiProxy::initLabels()
{
    ADD_WIDGET(Label,Label_Status,createLabel(MM::Text::Empty))
}

QMenu* MmUiProxy::createMenu(const QString &strTitle, const QIcon& icon)
{
    PYMenu* menu = new PYMenu(_parent);
    menu->setIcon(icon);
    menu->setTitle(strTitle);
    return menu;
}

QToolBar* MmUiProxy::createToolBar(const QString &strTitle)
{
    QToolBar* bar = new QToolBar(_parent);
    bar->setWindowTitle(strTitle);
    return bar;
}

QAction* MmUiProxy::createAction(const QString &strText,const QIcon &icon, const QString &strShortcut, bool checkable)
{
    QAction* act = new QAction(_parent);
    act->setText(strText);
    act->setIcon(icon);
    act->setShortcut(strShortcut);
    act->setCheckable(checkable);

    return act;
}

QAction* MmUiProxy::createAction(const QString &strText, bool checkable)
{
    QAction* act = new QAction(_parent);
    act->setText(strText);
    act->setCheckable(checkable);

    return act;
}

QPushButton* MmUiProxy::createPushButton(const QString &strText, const QIcon& icon)
{
    QPushButton* button = new QPushButton(_parent);
    button->setText(strText);
    button->setIcon(icon);
    return button;
}

QLabel* MmUiProxy::createLabel(const QString &strText)
{
    QLabel* label = new QLabel(_parent);
    label->setText(strText);
    return label;
}

QComboBox* MmUiProxy::createComboBox()
{
    QComboBox* box = new QComboBox(_parent);
    return box;
}

QLineEdit* MmUiProxy::createLineEdit()
{
    QLineEdit* edit = new QLineEdit(_parent);
    return edit;
}

QTimer* MmUiProxy::createTimer(int interval, bool singleShot)
{
    QTimer* timer = new QTimer(_parent);
    timer->setInterval(interval);
    timer->setSingleShot(singleShot);
    return timer;
}
