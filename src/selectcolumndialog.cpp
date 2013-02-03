#include "selectcolumndialog.h"
#include "mmdef.h"
#include "queryengine.h"
#include <QMessageBox>
#include "logutil.h"

SelectColumnDialog::SelectColumnDialog(QWidget *parent) :
    QDialog(parent)
{
    initVars();
    initWidgets();
    initSettings();
    loadColumns();
    initConnections();
}

///
/// PRIVATE UTILITY FUNCTIONS
///
QPushButton* SelectColumnDialog::createButton(const QString &strText)
{
    QPushButton* button = new QPushButton(strText,this);
    connect(button,SIGNAL(clicked()),this,SLOT(slot_buttonHandler()));
    return button;
}

void SelectColumnDialog::loadColumns()
{
    _indicator->show();
    _statusLabel->setText(tr("Loading columns ..."));

    QueryEngine* engine = QueryEngine::getInstance();
    connect(engine,SIGNAL(getColumns(QStringList)),this,SLOT(slot_getColumns(QStringList)));
    engine->startQueryColumns(MM::Constant::ProcessPerfClass);
}

void SelectColumnDialog::initVars()
{
    _columns.clear();
    foreach(const QString& strColumn,MM::Constant::MustHaveColums)
    {
        _columns << strColumn;
    }

    _columnsCopy.clear();
    foreach(const QString& strColumn,MM::Constant::DefaultColumns)
    {
        _columnsCopy << strColumn;
    }
}

void SelectColumnDialog::initWidgets()
{
    _mainLayout = new QVBoxLayout(this);

    _tipLabel = new MmTipLabel(tr("Related columns:"),this);
    _columnList = new QListWidget(this);
    _columnList->setAlternatingRowColors(true);
    _selectAllCheck = new QCheckBox(tr("Check/Uncheck All"),this);


    _buttonLayout = new QHBoxLayout;
    _indicator = new QProgressIndicator(this);
    _indicator->startAnimation();
    _statusLabel = new QLabel(this);

    _okButton = createButton(tr("Ok"));
    _cancelButton = createButton(tr("Cancel"));

    _buttonLayout->addWidget(_indicator);
    _buttonLayout->addWidget(_statusLabel);
    _buttonLayout->addStretch();
    _buttonLayout->addWidget(_cancelButton);
    _buttonLayout->addWidget(_okButton);

    _mainLayout->addWidget(_tipLabel);
    _mainLayout->addWidget(_columnList);
    _mainLayout->addWidget(_selectAllCheck);
    _mainLayout->addLayout(_buttonLayout);
}

void SelectColumnDialog::initSettings()
{
    setWindowTitle(MM::Text::SelectColumn);
    setModal(true);
}

void SelectColumnDialog::initConnections()
{
    connect(_columnList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(slot_itemClicked(QListWidgetItem*)));
    connect(_selectAllCheck,SIGNAL(stateChanged(int)),this,SLOT(slot_checkHandler(int)));
}
void SelectColumnDialog::assignToCopy()
{
    _columnsCopy.clear();

    foreach(const QString& str,_columns)
    {
        _columnsCopy << str;
    }
}

void SelectColumnDialog::recoverToCopy()
{
    _columns.clear();

    foreach(const QString& str,_columnsCopy)
    {
        _columns << str;
    }

    for(int i = 0;i < _columnList->count();i++)
    {
        if(_columns.contains(_columnList->item(i)->text()))
        {
            _columnList->item(i)->setCheckState(Qt::Checked);
        }
        else
        {
            _columnList->item(i)->setCheckState(Qt::Unchecked);
        }
    }

}

void SelectColumnDialog::getSelectedColumns()
{
    _columns.clear();

    for(int i = 0;i < _columnList->count();i++)
    {
        if(_columnList->item(i)->checkState() == Qt::Checked)
        {
            _columns << _columnList->item(i)->text();
        }
    }
}

void SelectColumnDialog::checkAllItems(int state)
{
    Qt::CheckState checkState = state == Qt::Checked ? Qt::Checked : Qt::Unchecked;

    for(int i = 0;i < _columnList->count();i++)
    {
        if(MM::Constant::MustHaveColums.contains(_columnList->item(i)->text()))
        {
            _columnList->item(i)->setCheckState(Qt::Checked);
        }
        else if(_columnList->item(i)->text().contains("commandline",Qt::CaseInsensitive))
        {
            _columnList->item(i)->setCheckState(Qt::Unchecked);
        }
        else
        {
            _columnList->item(i)->setCheckState(checkState);
        }
    }
}

///
/// REIMPL FUNCTIONS
///
void SelectColumnDialog::showEvent(QShowEvent *)
{
//    if(_columnList->count() == 0)
//    {
//        loadColumns();
//    }
}

///
/// PRIVATE SLOT FUNCTIONS
///
void SelectColumnDialog::slot_buttonHandler()
{
    QPushButton* who = qobject_cast<QPushButton*>(sender());

    if(who == _okButton)
    {
        getSelectedColumns();
        emit sig_setColumns(_columns);
        assignToCopy();
    }

    if(who == _cancelButton)
    {
        recoverToCopy();
    }

    close();
}


void SelectColumnDialog::slot_getColumns(const QStringList &columns)
{
    // if already loaded then returns
    if(_columnList->count() != 0)
    {
        return ;
    }

    _indicator->hide();
    _statusLabel->setText(QString());

    _columnList->clear();
    foreach(const QString& str,columns)
    {
        QListWidgetItem* colItem = new QListWidgetItem;
        colItem->setText(str);

        if(MM::Constant::MustHaveColums.contains(str,Qt::CaseInsensitive) || MM::Constant::DefaultColumns.contains(str,Qt::CaseInsensitive))
        {
            colItem->setCheckState(Qt::Checked);
        }
        else
        {
            colItem->setCheckState(Qt::Unchecked);
        }

        _columnList->addItem(colItem);


    }
}

void SelectColumnDialog::slot_itemClicked(QListWidgetItem *item)
{
    if(MM::Constant::MustHaveColums.contains(item->text()))
    {
        item->setCheckState(Qt::Checked);
        return;
    }

    if(item->text().contains("commandline",Qt::CaseInsensitive))
    {
        item->setCheckState(Qt::Unchecked);
        _statusLabel->setText("CommandLine is forbidden now !");
    }
    else
    {
        _statusLabel->setText(QString());
    }

    if(item->checkState() == Qt::Checked)
    {
        if(!_columns.contains(item->text()))
        {
            _columns.push_back(item->text());
        }
    }
    else
    {
        if(_columns.contains(item->text()))
        {
            int index = _columns.indexOf(item->text());
            _columns.removeAt(index);
        }
    }
}

void SelectColumnDialog::slot_checkHandler(int state)
{
    QCheckBox* who = qobject_cast<QCheckBox*>(sender());

    if(who == _selectAllCheck)
    {
        checkAllItems(state);
    }
}
