TEMPLATE = lib
QT += widgets

LIB_DEPENDS = \
    Core

HEADERS += \
    Widget.h

SOURCES += \
    Widget.cpp

include($$OUT_PWD/.config.pri)
