#ifndef XPROCESSTABLE_H
#define XPROCESSTABLE_H

#include <QWidget>
#include <QIcon>
#include <QAction>
#include <QMenu>

#include "itemwidgetfactory.h"
#include "xprocesstableheader.h"
#include "xheader.h"

#ifndef Container
#define Container QList
#endif

class QPainter;
class QPaintEvent;

class XProcessItem;
class XProcessTablePrivate;

class XProcessTable : public QWidget
{
    Q_DECLARE_PRIVATE(XProcessTable)
    Q_OBJECT

    enum Actions
    {
        AutoAdjust = 0,
        ActionCount
    };

public:
    explicit XProcessTable(QWidget *parent = 0);

public:
    void setColumns(const QStringList& colList);
    void sortByColumns(const QStringList& colList);

    void addProcess(XProcessItem* item);
    void addProcess(uint32_t pid);
    void updateProcessInfo(uint32_t pid,const QStringList& infoList);
    void deleteProcess(uint32_t pid);

    void setHorizontalLabels(const QStringList& labels);

    XProcessTablePrivate* d();

    void clear();

    void setAutoAdjust(bool adjust);
    bool isAutoAdjust() const;

    QByteArray contents();

    void find(const QString& expr);

    void setDataCount(int dataCnt);

protected:
    void contextMenuEvent(QContextMenuEvent *);

private:
    void initContextMenu();
    void createItemById(uint32_t pid);
    QAction* createAction(Actions act, const QString& strText, const QIcon& icon = QIcon());

private Q_SLOTS:
    void slot_actionHandler();

private:
    XHeader* header;
    XProcessTablePrivate* d_ptr;

    QVector<uint32_t> _pids;
    QMap<uint32_t,XProcessItem*> _pid2ItemMap;

    QMap<uint32_t,bool> _pid2UpdatedMap;
    QVector<uint32_t> _deadPids;

    QMenu* _contextMenu;
    QAction* _actions[ActionCount];

    Q_DISABLE_COPY(XProcessTable)

Q_SIGNALS:
    void sig_removeProcess(uint32_t pid);

};

class XProcessItem : public QObject
{
    Q_OBJECT
public:
    explicit XProcessItem(const QStringList& columns,int pidColumnIndex,int nameColumnIndex,QWidget* parent = 0);
    explicit XProcessItem(QWidget* parent = 0);
    ~XProcessItem();
public:

    void setName(const QString& strName);
    QString name() const;

    void setPid(uint32_t pid);
    uint32_t pid() const;

    void setRect(const QRectF& rect);
    QRectF rect() const;

    void setExpanded(bool expand);
    bool expanded() const;

    void setIcon(const QIcon& icon);
    QIcon icon() const;

    void resetColumns();
    void clearChildren();

    void addChild(XProcessItem* child);
    bool hasChild() const;
    Container<XProcessItem*> children();

    Container<BaseDisplayWidget*> getColumnWidgets() const;

    void updateInfo(const QStringList& infoList);

    void createWidgetByColumns(const QStringList& columns);

    // the following two columns are reserved
    void setPidColumnIndex(int index);
    int pidColumnIndex() const;

    void setNameColumnIndex(int index);
    int nameColumnIndex() const;

    void clearHistoryData();

    void showPopup(bool show);

    QString contents();

    void find(const QString& expr);

    void setDataCount(int dataCnt);

private:
    QWidget* _parent;
    QString _name;
    QRectF _expandboxRect;
    bool _expanded;
    uint32_t _pid;
    QIcon _icon;
    Container<BaseDisplayWidget*> _widgets;
    Container<XProcessItem* > _children;

    int _pidColumnIndex;
    int _nameColumnIndex;

    friend class XProcessTable;

    Q_DISABLE_COPY(XProcessItem)
};

#endif // XPROCESSTABLE_H
