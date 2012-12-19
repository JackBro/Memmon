#ifndef INFOQUERYWINDOW_H
#define INFOQUERYWINDOW_H
#include <QMainWindow>
#include <QWidget>
#include <QGroupBox>
#include <QComboBox>
#include <QListWidget>
#include <QTimer>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QSplitter>
#include <QTableWidget>
#include <QTextEdit>
#include <QDockWidget>
#include <QCheckBox>

#include "queryengine.h"
#include "infooutputdock.h"
#include "waitingwidget.h"

class InfoQueryWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit InfoQueryWindow(QWidget *parent = 0);
    
private:

    void initVars();
    void initWidgets();
    void initSettings();
    void initDockWindows();
    void initConnections();
    void populateClassCombo(QComboBox* list,const QStringList& classList);
    void initTimer();
    void reset();
    void setupQueryEngineHandler(QueryEngine* engine);
    void checkAllItems(bool checked);

//    QVBoxLayout* _mainLayout;
    QSplitter* _splitter;
    QWidget* _leftWidget;

    QVBoxLayout* _leftLayout;
    QCheckBox* _selectAllCheck;
    QComboBox* _classCombo;
    QListWidget* _columnList;
    QGroupBox* _autoBox;
    QVBoxLayout* _autoBoxLayout;
    QLabel* _intervalLabel;
    QComboBox* _intervalCombo;
    QTableWidget* _table;
    QTimer* _updateTimer;

    QString _class;
    QStringList _queryColumns;

    int _updateInterval;

    InfoOutputDock* _outputDock;
private Q_SLOTS:
    void checkboxHandler(int state);
    void updateInfo();
    void queryInfoReady(const QByteArray& result);
    void getQueryError(const QByteArray& error);
    void loadColumns(const QStringList& columns);
    void queryChanged(QListWidgetItem* item);
    void queryClassChanged(const QString& className);
    void updateIntervalChanged(int index);
    void enableAutoUpdate(bool enable);
};

#endif // INFOQUERYWINDOW_H
