#ifndef MODULEINFODOCK_H
#define MODULEINFODOCK_H

#include <QListWidget>

#include "notifydockwidget.h"
#include "./ctrl/searchedit.h"

class ModuleInfoDock : public NotifyDockWidget
{
    Q_OBJECT
public:
    explicit ModuleInfoDock(QWidget *parent = 0);

public Q_SLOTS:
    void setModuleList(const QStringList& moduleList);


private Q_SLOTS:
    void slot_findModule(const QString& str);

private:
    void initWidgets();
    void addToList(const QStringList& moduleList);
    inline void reset() { _modules.clear(); _moduleList->clear(); }

private:
    QVector<QString> _modules;
    SearchEdit* _searchEdit;
    QListWidget* _moduleList;
    QLabel* _totalCountLabel;

};

#endif // MODULEINFODOCK_H
