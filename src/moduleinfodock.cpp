#include "moduleinfodock.h"

#include <QVBoxLayout>

static const QString kTitle("Module List");
static const int kMargin = 5;

ModuleInfoDock::ModuleInfoDock(QWidget *parent) :
    NotifyDockWidget(parent)
{
    initWidgets();
}

/*!
 * private utility
 */
void ModuleInfoDock::initWidgets()
{
    _searchEdit = new SearchEdit(this);
    _moduleList = new QListWidget(this);
    _totalCountLabel = new QLabel(this);
    _moduleList->setAlternatingRowColors(true);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(kMargin, kMargin, kMargin, kMargin);
    mainLayout->addWidget(_searchEdit);
    mainLayout->addWidget(_moduleList);
    mainLayout->addWidget(_totalCountLabel);
    QWidget* thisWidget = new QWidget(this);
    thisWidget->setLayout(mainLayout);
    setWidget(thisWidget);

    connect(_searchEdit, SIGNAL(textChanged(QString)), this, SLOT(slot_findModule(QString)));

    setWindowTitle(kTitle);
}

void ModuleInfoDock::addToList(const QStringList &moduleList)
{
    reset();
    _totalCountLabel->setText(tr("Total Count: %1").arg(moduleList.size()));
    _moduleList->addItems(moduleList);
    foreach(const QString& strMod, moduleList)
    {
        _modules.push_back(strMod);
    }
}

/*!
 * private slots
 */
void ModuleInfoDock::slot_findModule(const QString &str)
{
    _moduleList->clear();
    if(str.isEmpty())
    {
        _moduleList->addItems(_modules.toList());
        _totalCountLabel->setText(tr("Total Count: %1").arg(_modules.size()));
    }
    else
    {
        for(int i = 0 ; i < _modules.size(); i++)
        {
            if(_modules.at(i).contains(str, Qt::CaseInsensitive))
            {
                _moduleList->addItem(_modules.at(i));
            }
        }

        QString info;
        info = tr("Total Count:%1 Sorted: %2").arg(_modules.size()).arg(_moduleList->count());
        _totalCountLabel->setText(info);
    }

}

/*!
 * public functions
 */
void ModuleInfoDock::setModuleList(const QStringList &moduleList)
{
    addToList(moduleList);
}
