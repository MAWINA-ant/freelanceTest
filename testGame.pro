#-------------------------------------------------
#
# Project created by QtCreator 2017-05-23T10:17:47
#
#-------------------------------------------------

QT       += core gui widgets multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testGame
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    mainmenu.cpp \
    myobject.cpp \
    inventory.cpp

HEADERS  += dialog.h \
    mainmenu.h \
    myobject.h \
    inventory.h

RESOURCES += \
    objects.qrc

