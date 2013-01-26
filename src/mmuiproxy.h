#ifndef MMUIPROXY_H
#define MMUIPROXY_H

#include <QObject>
#include <QWidget>
#include <QMap>
#include <QLabel>
#include <QToolBar>
#include <QAction>
#include <QLineEdit>
#include <QComboBox>
#include <QMenu>
#include <QPushButton>

#include "../ctrl/pyprog.h"
#include "../ctrl/pymenu.h"
#include "mmdef.h"

#undef PROXY_INLINE
#undef DECLARE_WIDGET_MAP
#undef GET_WIDGET_MAP
#undef DECLARE_INLINE_WIDGET_COUNT_ACCESSOR
#undef DECLARE_INLINE_ADD_WIDGET_TO_MAP_ACCESSOR
#undef DECLARE_INLINE_GET_WIDGET_ACCESSOR
#undef ADD_WIDGET
#undef ADD_WIDGET

#define PROXY_INLINE inline
#define DECLARE_WIDGET_MAP(WIDGET_NAME) QMap<WIDGET_NAME,Q ## WIDGET_NAME*> _ ## WIDGET_NAME ## Map;
#define GET_WIDGET_MAP(WIDGET_NAME) _ ## WIDGET_NAME ## Map

#define DECLARE_INLINE_WIDGET_COUNT_ACCESSOR(WIDGET_NAME) \
    PROXY_INLINE int WIDGET_NAME ## Count() const \
{ \
    return _ ## WIDGET_NAME ## Map.size(); \
    }


#define DECLARE_INLINE_ADD_WIDGET_TO_MAP_ACCESSOR(WIDGET_NAME) \
    PROXY_INLINE void add ## WIDGET_NAME (WIDGET_NAME enum_,Q##WIDGET_NAME* widget) \
{ \
    if(!_ ## WIDGET_NAME ## Map.contains(enum_)) \
{ \
    _ ## WIDGET_NAME ## Map.insert(enum_,widget); \
    } \
    }

#define DECLARE_INLINE_GET_WIDGET_ACCESSOR(WIDGET_NAME) \
    PROXY_INLINE Q ## WIDGET_NAME* get ## WIDGET_NAME(WIDGET_NAME enum_) \
{ \
    if(GET_WIDGET_MAP(WIDGET_NAME).contains(enum_)) \
{ \
    return GET_WIDGET_MAP(WIDGET_NAME)[enum_]; \
    } \
    else \
{ \
    return NULL; \
    } \
    }


#define ADD_WIDGET(WIDGET_NAME,enum_,widget) add ## WIDGET_NAME(enum_,widget);
#define GET_WIDGET(WIDGET_NAME,enum_) Q##WIDGET_NAME* get ## WIDGET_NAME(enum_)

class MmUiProxy : public QObject
{
    Q_OBJECT
public:

    enum Menu
    {

    };

    enum Action
    {

    };

    enum ToolBar
    {

    };

    enum Label
    {
        Label_Status
    };

    enum Widget
    {
        Widget_CpuIndicator,
        Widget_MemIndicator
    };

    explicit MmUiProxy(QWidget *parent = 0);
    
public:
    DECLARE_INLINE_GET_WIDGET_ACCESSOR(Widget)
    DECLARE_INLINE_GET_WIDGET_ACCESSOR(Label)

    DECLARE_INLINE_WIDGET_COUNT_ACCESSOR(Widget)
    DECLARE_INLINE_WIDGET_COUNT_ACCESSOR(Label)

private:
    void initWidgets();
    void initLabels();


    // utility function for constructing qaction
    QMenu* createMenu(const QString& strTitle, const QIcon& icon = QIcon());
    QToolBar* createToolBar(const QString& strTitle);
    QAction* createAction(const QString& strText,
                          const QIcon& icon = QIcon(),
                          const QString& strShortcut = QString(),
                          bool checkable = false);
    QAction* createAction(const QString& strText,bool checkable);
    QPushButton* createPushButton(const QString& strText, const QIcon& icon = QIcon());
    QLabel* createLabel(const QString& strText = QString());
    QComboBox* createComboBox();
    QLineEdit* createLineEdit();
    QTimer* createTimer(int interval = 0, bool singleShot = false);

    DECLARE_INLINE_ADD_WIDGET_TO_MAP_ACCESSOR(Widget)
    DECLARE_INLINE_ADD_WIDGET_TO_MAP_ACCESSOR(Label)

private:
    QWidget* _parent;

    DECLARE_WIDGET_MAP(Widget)
    DECLARE_WIDGET_MAP(Label)


};

#endif // MMUIPROXY_H
