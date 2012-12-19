#include "qradiolist.h"

QRadioList::QRadioList(QWidget *parent) :
    QWidget(parent)
{
    initLayout();
}


///
/// PRIVATE UTILITY FUNCTIONS
///
void QRadioList::initLayout()
{
    _mainLayout = new QVBoxLayout(this);
    _mainLayout->setContentsMargins(0,0,0,0);
    _area = new QScrollArea(this);
    _area->setFrameShape(QFrame::NoFrame);
    _area->setWidgetResizable(true);
    _area->setContentsMargins(0,0,0,0);

    _mainLayout->addWidget(_area);

    _hostWidget = new QWidget(this);
    _hostWidgetLayout = new QVBoxLayout(_hostWidget);
    _hostWidgetLayout->setContentsMargins(0,0,0,0);
    _hostWidgetLayout->setSpacing(0);
    _area->setWidget(_hostWidget);
}

QRadioButton* QRadioList::createRadioByName(const QString &strText)
{
    QRadioButton* newRadio = new QRadioButton(strText,this);
    newRadio->setFixedHeight(QRL::ItemHeight);
    connect(newRadio,SIGNAL(clicked()),this,SLOT(slog_radioHandler()));
    _hostWidgetLayout->addWidget(newRadio);

    _buttons.push_back(newRadio);

    if(_count % 2 == 0)
    {
        newRadio->setStyleSheet("background-color:white;");
    }
    else
    {
        newRadio->setStyleSheet("background-color:lightgray;");
    }
    ++_count;

    return newRadio;
}



///
/// PRIVATE SLOT FUNCTIONS
///
void QRadioList::slog_radioHandler()
{
    QRadioButton* who = qobject_cast<QRadioButton*>(sender());
    emit itemClicked(who->text());
    emit itemClicked(who);
}

///
/// PUBLIC FUNCTIONS
///
void QRadioList::addItem(const QString& strItem)
{
    QRadioButton* radio = createRadioByName(strItem);
    _hostWidgetLayout->addWidget(radio);
}

void QRadioList::clear()
{
    QLayoutItem *child;
    while ((child = _hostWidgetLayout->takeAt(0)) != 0) {
      delete child;
    }

    qDeleteAll(_buttons);
    _buttons.clear();

    _count = 0;
}

int QRadioList::count()
{
    return _count;
}

void QRadioList::stop()
{
    _hostWidgetLayout->addStretch();
}
