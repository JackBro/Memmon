#ifndef MMUIPROXY_H
#define MMUIPROXY_H

#include <QObject>
#include <QWidget>

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

    enum Widget
    {

    };

    explicit MmUiProxy(QWidget *parent = 0);
    
signals:
    
public slots:
    
};

#endif // MMUIPROXY_H
