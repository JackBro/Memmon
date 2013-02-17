#ifndef ITEMWIDGETFACTORY_H
#define ITEMWIDGETFACTORY_H

#include <QLabel>
#include <QObject>
#include <QToolButton>
#include <QLCDNumber>

#include "../ctrl/progressbar.h"
#include "../ctrl/pyprog.h"

enum WidgetType
{
    Text,Bytes,Progress,Path,Icon,Time,Number
};

static const int DefaultDataCount = 10;

template <typename T>
class AbstractItemWidget
{
public:
    AbstractItemWidget(){}

protected:
    virtual void setValue(const T& value) = 0;
    virtual T value() const = 0;
    virtual WidgetType widgetType() const = 0;
    virtual QString text() const = 0;
    virtual bool find(const QString& expr) = 0;

protected:
    T _value;

};

class BaseDisplayWidget : public QWidget, AbstractItemWidget<QString>
{
    Q_OBJECT
public:
    explicit BaseDisplayWidget(QWidget* parent = 0);

public:
    virtual void setValue(const QString &value);
    virtual QString value() const;
    virtual WidgetType widgetType() const;
    virtual bool find(const QString& expr);
    inline void setColumnName(const QString& strName){ _columnName = strName; }
    inline QString columnName() const { return _columnName; }
    inline void setProcessName(const QString& strProcessName){ _processName = strProcessName; }
    inline QString processName() const { return _processName; }
    virtual QString text() const { return QString(); }
protected:
    QString _columnName;
    QString _processName;

};

class TextDisplayWidget : public BaseDisplayWidget
{
    Q_OBJECT

public:
    explicit TextDisplayWidget(QWidget* parent = 0);

public:
    void setValue(const QString &value);
    QString value() const;
    WidgetType widgetType() const;
    QString text() const;
    bool find(const QString &expr);

protected:
    bool event(QEvent *);


private:
    QLabel* _label;

};

class IconDisplayWidget : public BaseDisplayWidget
{
    Q_OBJECT
public:
    explicit IconDisplayWidget(QWidget* parent = 0);

public:
    void setValue(const QString &value);
    QString value() const;

    WidgetType widgetType() const;

    void setIcon(const QIcon& icon);
    QIcon icon() const;

    QString text() const;

    bool find(const QString &expr);

private:
    QIcon _icon;
    QLabel* _iconLabel;
    QLabel* _label;
};

class ProgressDisplayWidget : public BaseDisplayWidget
{
    Q_OBJECT
public:
    explicit ProgressDisplayWidget(QWidget* parent = 0);

public:
    void setValue(const QString& value);
    QString value() const;
    WidgetType widgetType() const;

    QString text() const;

private:
    PYProg* _pgsBar;

};

class BytesDisplayWidget : public BaseDisplayWidget
{
    Q_OBJECT
public:
    explicit BytesDisplayWidget(QWidget* parent = 0);

public:
    void setValue(const QString &value);
    QString value() const;
    WidgetType widgetType() const;

    QString text() const;

    bool find(const QString &expr);

    void clear();

    void showPopup(bool show);
    bool isPopupShown() const;

    void setDataCount(int cnt);
    int dataCount() const;

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

private:
    void showDataChart();
    inline void updateChartCaption();
    void updateBufferSize();

    QLabel* _label;
    QList<qreal> _data;

    bool _showPopup;
    bool _enter;

    int _dataCnt;
};

class PathDisplayWidget : public BaseDisplayWidget
{
    Q_OBJECT
public:
    explicit PathDisplayWidget(QWidget* parent = 0);

public:
    void setValue(const QString &value);
    QString value() const;
    WidgetType widgetType() const;

    QString text() const;
    bool find(const QString &expr);


private:
    QToolButton* _button;

private Q_SLOTS:
    void slot_showPath();
};

class TimeDisplayWidget : public BaseDisplayWidget
{
    Q_OBJECT
public:
    explicit TimeDisplayWidget(QWidget* parent = 0);

public:
    void setValue(const QString &value);
    QString value() const;
    WidgetType widgetType() const;
    QString text() const;

private:
    void displayTime();

    QLCDNumber* _lcd;
    QString _value;
};

class NumberDisplayWidget : public BaseDisplayWidget
{
    Q_OBJECT
public:
    explicit NumberDisplayWidget(QWidget* parent = 0);

public:
    void setValue(const QString &value);
    QString value() const;
    WidgetType widgetType() const;
    QString text() const;
    bool find(const QString &expr);

private:
    QLabel* _label;
};

class ItemWidgetFactory
{
private:
    ItemWidgetFactory();

public:
    static BaseDisplayWidget* makeWidget(WidgetType type,QWidget* parent = 0);
    static BaseDisplayWidget* makeWidgetByName(const QString& strColumnName,QWidget* parent = 0);

private:
    static QStringList TextList;
    static QStringList BytesList;
    static QStringList ProgressList;
    static QStringList PathList;
    static QStringList IconList;
    static QStringList TimeList;
    static QStringList NumberList;

};

#endif // ITEMWIDGETFACTORY_H
