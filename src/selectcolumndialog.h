#ifndef SELECTCOLUMNDIALOG_H
#define SELECTCOLUMNDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>

#include "ctrl/llnotifywidget.h"
#include "queryengine.h"
#include "QProgressIndicator.h"


class SelectColumnDialog : public LLNotifyWidget
{
    Q_OBJECT
public:
    explicit SelectColumnDialog(QWidget *parent = 0);
    
    void fire_setColumns();

    void reload();

    void setQueryEngine(const QString& strQueryEngine);
    QString queryEngine() const;

protected:
    QSize sizeHint() const
    {
        return QSize(300,400);
    }

    void showEvent(QShowEvent *);

private:
    QPushButton* createButton(const QString& strText);
    void loadColumns();
    void initVars();
    void initWidgets();
    void initSettings();
    void initConnections();
    void assignToCopy();
    void recoverToCopy();
    void getSelectedColumns();
    void checkAllItems(int state);

private:

    QListWidget* _columnList;
    QLabel* _tipLabel;
    QCheckBox* _selectAllCheck;
    QVBoxLayout* _mainLayout;
    QHBoxLayout* _buttonLayout;
    QProgressIndicator* _indicator;
    QLabel* _statusLabel;
    QPushButton* _okButton;
    QPushButton* _cancelButton;

    QStringList _columns;
    QStringList _columnsCopy;

    QString _strQueryEngine;

private Q_SLOTS:
    void slot_buttonHandler();
    void slot_getColumns(const QStringList& columns);
    void slot_itemClicked(QListWidgetItem* item);
    void slot_checkHandler(int state);

Q_SIGNALS:
    void sig_setColumns(const QStringList& columns);

    
};

#endif // SELECTCOLUMNDIALOG_H
