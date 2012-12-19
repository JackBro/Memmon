#include "infoquerywindow.h"
#include "logutil.h"
#include <QFile>
#include <QPointer>
#include <QHeaderView>
#include "mmdef.h"

#define MARGIN 3

QStringList getAllClasses()
{
    QStringList classes;
    QFile classFile(":/res/classes.txt");
    if(!classFile.open(QIODevice::ReadOnly))
    {
        return classes;
    }

    QByteArray allClasses = classFile.readAll();
    QString strAllClasses(allClasses);
    return strAllClasses.split('\n');
}

InfoQueryWindow::InfoQueryWindow(QWidget *parent) :
    QMainWindow(parent)
{
    initVars();
    initWidgets();
    initDockWindows();
    initSettings();
    initConnections();
}


///
/// private functions
///
void InfoQueryWindow::initVars()
{
    _updateTimer = 0;
    _updateInterval = 1000;
}

void InfoQueryWindow::initWidgets()
{

    _leftLayout = new QVBoxLayout;
    _leftLayout->setContentsMargins(MARGIN,MARGIN,MARGIN,MARGIN);
    _selectAllCheck = new QCheckBox(tr("Select/Unselect All"),this);
    _classCombo = new QComboBox(this);

    _columnList = new QListWidget(this);
    _autoBox = new QGroupBox(tr("Auto Update"),this);
    _autoBoxLayout = new QVBoxLayout;
    _intervalLabel = new QLabel(tr("Update interval:"),this);
    _intervalCombo = new QComboBox(this);
    _table = new QTableWidget(this);

    // setup layout's widgets
    _leftLayout->addWidget(_selectAllCheck);
    _leftLayout->addWidget(_classCombo);
    _leftLayout->addWidget(_columnList);
    _leftLayout->addWidget(_autoBox);

    // setup autobox
    _autoBoxLayout->addWidget(_intervalLabel);
    _autoBoxLayout->addWidget(_intervalCombo);
    _autoBox->setLayout(_autoBoxLayout);
    _autoBox->setCheckable(true);

    // setup left widget's layout
    _leftWidget = new QWidget(this);
    _leftWidget->setLayout(_leftLayout);

    // setup splitter
    _splitter = new QSplitter(Qt::Horizontal,this);
    _splitter->addWidget(_leftWidget);
    _splitter->addWidget(_table);

    // set up mainlayout
    setCentralWidget(_splitter);
}

void InfoQueryWindow::initSettings()
{
    _autoBox->setChecked(false);

    _table->setAlternatingRowColors(true);
    _table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _table->setSelectionBehavior(QAbstractItemView::SelectRows);
    _table->verticalHeader()->hide();

    _splitter->setHandleWidth(1);
    _columnList->setAlternatingRowColors(true);
    populateClassCombo(_classCombo,getAllClasses());

    _intervalCombo->addItem("1000 ms");
    _intervalCombo->addItem("3000 ms");
    _intervalCombo->addItem("5000 ms");
}

void InfoQueryWindow::initDockWindows()
{
    _outputDock = new InfoOutputDock(tr("Output"),this);
    _outputDock->setAllowedAreas(Qt::AllDockWidgetAreas);
    addDockWidget(Qt::BottomDockWidgetArea,_outputDock);
}

void InfoQueryWindow::initConnections()
{
    connect(_columnList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(queryChanged(QListWidgetItem*)));
    connect(_intervalCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(updateIntervalChanged(int)));
    connect(_classCombo,SIGNAL(currentIndexChanged(QString)),this,SLOT(queryClassChanged(QString)));
    connect(_autoBox,SIGNAL(clicked(bool)),this,SLOT(enableAutoUpdate(bool)));

    connect(_selectAllCheck,SIGNAL(stateChanged(int)),this,SLOT(checkboxHandler(int)));
}

void InfoQueryWindow::populateClassCombo(QComboBox *list,const QStringList& classList)
{
    if(!classList.isEmpty())
    {
        // set default class name
        _class = classList.at(0).trimmed();
    }

    foreach(const QString& strClass,classList)
    {
        _classCombo->addItem(strClass.trimmed());
    }

    _classCombo->setCurrentIndex(10);
}

void InfoQueryWindow::initTimer()
{
    if(_updateTimer == NULL)
    {
        _updateTimer = new QTimer(this);
        _updateTimer->setInterval(_updateInterval);
        connect(_updateTimer,SIGNAL(timeout()),this,SLOT(updateInfo()));
    }
}

void InfoQueryWindow::reset()
{
    _queryColumns.clear();
    _table->clear();
    _table->setColumnCount(0);
    _columnList->clear();

    disconnect(_selectAllCheck,SIGNAL(stateChanged(int)),this,SLOT(checkboxHandler(int)));
    _selectAllCheck->setCheckState(Qt::Unchecked);
    connect(_selectAllCheck,SIGNAL(stateChanged(int)),this,SLOT(checkboxHandler(int)));
}

void InfoQueryWindow::setupQueryEngineHandler(QueryEngine *engine)
{
    connect(engine,SIGNAL(getColumns(QStringList)),this,SLOT(loadColumns(QStringList)));
    connect(engine,SIGNAL(queryResultReady(QByteArray)),this,SLOT(queryInfoReady(QByteArray)));
    connect(engine,SIGNAL(getQueryError(QByteArray)),this,SLOT(getQueryError(QByteArray)));
}

void InfoQueryWindow::checkAllItems(bool checked)
{
    int itemCount = _columnList->count();

    if(itemCount == 0)
    {
        return ;
    }

    for(int itemIndex = 0;itemIndex < itemCount;itemIndex++)
    {
        QListWidgetItem* item = _columnList->item(itemIndex);
        item->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
    }

    if(checked)
    {
        queryChanged(NULL);
    }
}

///
/// private slot functions
///
void InfoQueryWindow::checkboxHandler(int state)
{
    QCheckBox* who = qobject_cast<QCheckBox*>(sender());

    if(who == _selectAllCheck)
    {
        checkAllItems(state == Qt::Checked);
    }
}

void InfoQueryWindow::updateInfo()
{
    QStringList arguments;
    arguments << "path" << _class << "get";

    for(int i = 0;i < _queryColumns.size();i++)
    {
        if(i < (_queryColumns.size() - 1))
        {
            arguments << _queryColumns.at(i) << ",";
        }
        else
        {
            arguments << _queryColumns.at(i);
        }
    }

    arguments << "/format:csv";

    QueryEngine* engine = QueryEngine::getInstance();
    setupQueryEngineHandler(engine);
    engine->startQuery(arguments);

    WaitingWidget::showText("Querying command ...");

}

void InfoQueryWindow::queryInfoReady(const QByteArray &result)
{
    WaitingWidget::hide();

    QString strResult(result);
    LOG_VAR(strResult);
    QStringList results = strResult.split('\n');

    // discard header info
    results.pop_front();
    results.pop_front();
    results.pop_back();

    int rowCount = results.size();
    _table->setRowCount(rowCount);

    for(int rowIndex = 0;rowIndex < rowCount;rowIndex++)
    {
        _table->setRowHeight(rowIndex,MM::Constant::RowHeight);

        QString simpleLine = results.at(rowIndex).simplified();
        QStringList simpleList = simpleLine.split(',');
        for(int colIndex = 1;colIndex < simpleList.size();colIndex++)
        {
            QTableWidgetItem* item = new QTableWidgetItem;
            QString strText = simpleList.at(colIndex).isEmpty() ? "N/A" : simpleList.at(colIndex);
            item->setText(strText);

            _table->setItem(rowIndex,colIndex,item);
        }
    }
}

void InfoQueryWindow::getQueryError(const QByteArray &error)
{
    QString output;
    output += QString(error);
    output += "============================";
    _outputDock->append(output);
}

void InfoQueryWindow::loadColumns(const QStringList &columns)
{
    WaitingWidget::hide();

    _columnList->clear();

    foreach(const QString& strColumn,columns)
    {
        QListWidgetItem* columnItem = new QListWidgetItem;
        columnItem->setText(strColumn);
        columnItem->setCheckState(Qt::Unchecked);
        _columnList->addItem(columnItem);
    }
}
void InfoQueryWindow::queryChanged(QListWidgetItem *item)
{
    _queryColumns.clear();
    for(int i = 0;i < _columnList->count();i++)
    {
        if(_columnList->item(i)->checkState() == Qt::Checked)
        {
            if(!_queryColumns.contains(_columnList->item(i)->text()))
            {
                _queryColumns.push_back(_columnList->item(i)->text());
            }
        }
    }

    _table->setColumnCount(_queryColumns.size());
    _table->setHorizontalHeaderLabels(_queryColumns);
    LOG_VAR(_queryColumns);
    updateInfo();
}

void InfoQueryWindow::queryClassChanged(const QString &className)
{
    reset();
    _class = className;
    QueryEngine* engine = QueryEngine::getInstance();
    setupQueryEngineHandler(engine);
    engine->startQueryColumns(_class);

    WaitingWidget::showText("Querying columns ...");

}

void InfoQueryWindow::updateIntervalChanged(int index)
{
    initTimer();
    switch(index)
    {
    case 0:
        _updateTimer->setInterval(1000);
        break;
    case 1:
        _updateTimer->setInterval(3000);
        break;
    case 2:
        _updateTimer->setInterval(5000);
        break;
    }
}

void InfoQueryWindow::enableAutoUpdate(bool enable)
{
    initTimer();

    if(enable)
    {
        _updateTimer->start();
    }
    else
    {
        _updateTimer->stop();
    }
}
