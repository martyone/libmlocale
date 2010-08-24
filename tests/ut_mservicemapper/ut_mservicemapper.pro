include(../common_top.pri)

LIBS -= -lmeegotouchcore -lmeegotouchviews
SFWDIR = ../../mservicemapper
INCLUDEPATH += $$SFWDIR
DEPENDPATH += $$INCLUDEPATH
QT -= dbus svg network gui

CONFIG += mobility
MOBILITY += serviceframework

TARGET = ut_mservicemapper

# unit
TEST_SOURCES =  \
    $$SFWDIR/mservicemapper.cpp \
    $$SFWDIR/mservicemapper_p.cpp \

# unit test and unit
SOURCES += \
    $$TEST_SOURCES \
    ut_mservicemapper.cpp \

# unit test and unit
HEADERS += \
    ut_mservicemapper.h \
    $$SFWDIR/mservicemapper.h \
    $$SFWDIR/mservicemapper_p.h \

include(../common_bot.pri)
