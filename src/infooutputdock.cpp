#include "infooutputdock.h"
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

InfoOutputDock::InfoOutputDock(const QString &strTitle, QWidget *parent):NotifyDockWidget(strTitle,parent)
{
    initWidgets();
    initActions();
    initSettings();
    initConnections();
}

InfoOutputDock::InfoOutputDock(QWidget *parent) :
    NotifyDockWidget(parent)
{
    initWidgets();
    initActions();
    initSettings();
    initConnections();
}

///
/// private utility functions
///
void InfoOutputDock::initWidgets()
{
    _mainWidget = new QWidget(this);
    _widgetLayout = new QVBoxLayout;
    _widgetLayout->setSpacing(1);
    _widgetLayout->setContentsMargins(1,1,1,1);
    _toolbar = new QToolBar(this);
    _edit = new QTextEdit(this);
    _edit->setReadOnly(true);

    _widgetLayout->addWidget(_toolbar);
    _widgetLayout->addWidget(_edit);

    _mainWidget->setLayout(_widgetLayout);

    setWidget(_mainWidget);

}

void InfoOutputDock::initSettings()
{

}

void InfoOutputDock::initActions()
{
    _clearAct = createAction(tr("Clear"),QIcon(":/images/clear.png"),tr("clear the text"),this);
    _saveAct = createAction(tr("Save"),QIcon(":/images/save.png"),tr("Save the text to a file"),this);

    connect(_clearAct,SIGNAL(triggered()),this,SLOT(clear()));
    connect(_saveAct,SIGNAL(triggered()),this,SLOT(save()));

    _toolbar->addAction(_clearAct);
    _toolbar->addAction(_saveAct);
}

void InfoOutputDock::initConnections()
{

}

QAction* InfoOutputDock::createAction(const QString &strText, const QIcon &icon, const QString &strTip,QWidget* parent)
{
    QAction* act = new QAction(parent);
    act->setText(strText);
    act->setIcon(icon);
    act->setToolTip(strTip);
    return act;
}

///
/// private slot functions
///
void InfoOutputDock::clear()
{
    _edit->clear();
}

void InfoOutputDock::save()
{
    QString saveFile = QFileDialog::getSaveFileName(this,tr("Save As"));
    QString strContent = _edit->toPlainText();
    QFile sFile(saveFile);

    if(!sFile.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this,tr("Error"),tr("Failed to save output to file !"));
        return ;
    }

    QTextStream outStream(&sFile);
    outStream << strContent;
    sFile.close();
}

///
/// public functions
///
void InfoOutputDock::setText(const QString &str)
{
    _edit->setText(str);
}

QString InfoOutputDock::text() const
{
    return _edit->toPlainText();
}

void InfoOutputDock::append(const QString &str)
{
    _edit->append(str);
}
