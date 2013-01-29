DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD

HEADERS += $$PWD/sigar.h \
           $$PWD/sigar_fileinfo.h \
           $$PWD/sigar_format.h \
           $$PWD/sigar_log.h \
           $$PWD/sigar_ptql.h

LIBS += $$PWD/lib/sigar-x86-winnt.lib
