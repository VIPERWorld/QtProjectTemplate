
TEMPLATE = app
CONFIG += testcase
QT -= core gui
QT += testlib

DEPENDS = \
    Gui

SOURCES += \
    main.cpp

include($$OUT_PWD/AutoBuild.pri)