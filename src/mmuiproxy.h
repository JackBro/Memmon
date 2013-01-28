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
#include <QToolButton>

#include "../ctrl/pyprog.h"
#include "../ctrl/pymenu.h"
#include "../ctrl/pyhistory.h"
#include "mmdef.h"

#undef PROXY_INLINE
#undef DECLARE_WIDGET_MAP
#undef GET_WIDGET_MAP
#undef DECLARE_INLINE_WIDGET_COUNT_ACCESSOR
#undef DECLARE_INLINE_ADD_WIDGET_TO_MAP_ACCESSOR
#undef DECLARE_INLINE_GET_WIDGET_ACCESSOR
#undef ADD_WIDGET
#undef ADD_WIDGET
#undef ADD_WIDGET_TYPE

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

#define ADD_WIDGET_TYPE(WIDGET_NAME) public: \
                                        DECLARE_INLINE_GET_WIDGET_ACCESSOR(WIDGET_NAME) \
                                        DECLARE_INLINE_WIDGET_COUNT_ACCESSOR(WIDGET_NAME) \
                                     private: \
                                        DECLARE_INLINE_ADD_WIDGET_TO_MAP_ACCESSOR(WIDGET_NAME) \
                                        DECLARE_WIDGET_MAP(WIDGET_NAME)

#define DECLARE_CREATE_WIDGET_FUNC(WIDGET_NAME,PARAMS) Q ## WIDGET_NAME* create ## WIDGET_NAME PARAMS;



class Memmon;
class MmUiProxy : public QObject
{
    Q_OBJECT
public:

    enum Menu
    {
        Menu_File,
        Menu_Config,
        Menu_Window,
        Menu_About
    };

    enum Action
    {
        Action_Export,
        Action_Exit,
        Action_SelectColumns,
        Action_MemUtil,
        Action_LogDock,
        Action_WmiQuery,
        Action_Help,
        Action_AboutThis
    };

    enum ToolBar
    {
        ToolBar_Tool
    };

    enum Label
    {
        Label_Status,
        Label_UpdateInterval,
        Label_TotalProcessCount,
        Label_RunningServiceCount,
        Label_RunningDriverCount
    };

    enum Widget
    {
        Widget_CpuIndicator,
        Widget_MemIndicator,
        Widget_CpuUsageHistory,
        Widget_MemUsageHistory
    };

    enum ToolButton
    {
        ToolButton_Start,
        ToolButton_Stop,
        ToolButton_Clear,
        ToolButton_ShowPopup,
        ToolButton_GeneralInfo
    };

    enum ComboBox
    {
        ComboBox_UpdateInterval
    };

    explicit MmUiProxy(Memmon *parent = 0);
    
    ADD_WIDGET_TYPE(Widget)
    ADD_WIDGET_TYPE(Label)
    ADD_WIDGET_TYPE(Action)
    ADD_WIDGET_TYPE(Menu)
    ADD_WIDGET_TYPE(ToolBar)
    ADD_WIDGET_TYPE(ToolButton)
    ADD_WIDGET_TYPE(ComboBox)

private:
    void initWidgets();
    void initLabels();
    void initActions();
    void initMenus();
    void initToolBars();
    void initComboBoxes();
    void initToolButtons();

    // utility function for constructing qaction
    DECLARE_CREATE_WIDGET_FUNC(Menu,(const QString& strTitle, const QIcon& icon = QIcon()))
    DECLARE_CREATE_WIDGET_FUNC(ToolBar,(const QString& strTitle))
    DECLARE_CREATE_WIDGET_FUNC(Action,(const QString& strText,
                                       const QIcon& icon = QIcon(),
                                       const QString& strShortcut = QString(),
                                       bool checkable = false))
    DECLARE_CREATE_WIDGET_FUNC(Action,(const QString& strText,bool checkable))
    DECLARE_CREATE_WIDGET_FUNC(PushButton,(const QString& strText, const QIcon& icon = QIcon()))
    DECLARE_CREATE_WIDGET_FUNC(Label,(const QString& strText = QString()))
    DECLARE_CREATE_WIDGET_FUNC(ComboBox,())
    DECLARE_CREATE_WIDGET_FUNC(LineEdit,())
    DECLARE_CREATE_WIDGET_FUNC(Timer,(int interval = 0, bool singleShot = false))
    DECLARE_CREATE_WIDGET_FUNC(ToolButton,(const QString& strText, const QIcon& icon = QIcon()))

private:
    Memmon* _parent;

};

#endif // MMUIPROXY_H
