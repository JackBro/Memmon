#include "logoutputwindow.h"
#include "mmdef.h"

LogOutputWindow::LogOutputWindow(QWidget *parent) :
    NotifyDockWidget(parent)
{
    initLayout();
}

void LogOutputWindow::initLayout()
{
    _logEdit = new QTextEdit(this);
    setWidget(_logEdit);

    setWindowTitle(MM::Text::Title_LogOutput);
}

void LogOutputWindow::setText(const QString &strLog)
{
    _logEdit->setText(strLog);
}

void LogOutputWindow::appendText(const QString &strLog)
{
    _logEdit->append(strLog);
}

void LogOutputWindow::clear()
{
    _logEdit->clear();
}
