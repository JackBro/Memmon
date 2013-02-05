INCLUDEPATH = $$PWD
DEPENDPATH = $$PWD

# Input
SOURCES +=  $$PWD/main.cpp \
            $$PWD/memmon.cpp \
            $$PWD/mmdef.cpp \
            $$PWD/processtable.cpp \
            $$PWD/queryengine.cpp \
            $$PWD/querythread.cpp \
            $$PWD/infoquerywindow.cpp \
            $$PWD/infooutputdock.cpp \
            $$PWD/memchart.cpp \
            $$PWD/testwidget.cpp \
            $$PWD/pyhistogramtip.cpp \
            $$PWD/waitingwidget.cpp \
            $$PWD/qprogressindicator.cpp \
            $$PWD/mmutil.cpp \
            $$PWD/logutil.cpp \
            $$PWD/processdisplaywidget.cpp \
            $$PWD/selectcolumndialog.cpp \
            $$PWD/querymanager.cpp \
            $$PWD/memorymonitorwindow.cpp \
            $$PWD/qradiolist.cpp \
            $$PWD/memorymonitordock.cpp \
            $$PWD/logoutputwindow.cpp \
            $$PWD/notifydockwidget.cpp \
            $$PWD/infoquerydock.cpp \
            src/mmuiproxy.cpp \
            src/mmvarproxy.cpp \
            src/usageinfopad.cpp \

SOURCES += $$PWD/infofetcher/cpuusagefetcher.cpp \
           $$PWD/infofetcher/memoryusagefetcher.cpp \
           $$PWD/infofetcher/processcountfetcher.cpp \
           $$PWD/infofetcher/basefetcher.cpp \
           $$PWD/infofetcher/servicecountfetcher.cpp \
           $$PWD/infofetcher/drivercountfetcher.cpp \
           $$PWD/infofetcher/coreusagefetcher.cpp

HEADERS += \
            $$PWD/memmon.h \
            $$PWD/mmdef.h \
            $$PWD/processtable.h \
            $$PWD/queryengine.h \
            $$PWD/querythread.h \
            $$PWD/infoquerywindow.h \
            $$PWD/infooutputdock.h \
            $$PWD/memchart.h \
            $$PWD/memchart_p.h \
            $$PWD/testwidget.h \
            $$PWD/pyhistogramtip.h \
            $$PWD/waitingwidget.h \
            $$PWD/qprogressindicator.h \
            $$PWD/logutil.h \
            $$PWD/mmutil.h \
            $$PWD/processdisplaywidget.h \
            $$PWD/selectcolumndialog.h \
            $$PWD/querymanager.h \
            $$PWD/memorymonitorwindow.h \
            $$PWD/qradiolist.h \
            $$PWD/memorymonitordock.h \
            $$PWD/logoutputwindow.h \
            $$PWD/notifydockwidget.h \
            $$PWD/infoquerydock.h \
            $$PWD/notifydockwidget.h \
            src/mmuiproxy.h \
            src/mmvarproxy.h \
            src/usageinfopad.h \

HEADERS += $$PWD/infofetcher/cpuusagefetcher.h \
           $$PWD/infofetcher/memoryusagefetcher.h \
           $$PWD/infofetcher/processcountfetcher.h \
           $$PWD/infofetcher/basefetcher.h \
           $$PWD/infofetcher/servicecountfetcher.h \
           $$PWD/infofetcher/drivercountfetcher.h \
           $$PWD/infofetcher/coreusagefetcher.h

















