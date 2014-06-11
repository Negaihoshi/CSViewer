#-------------------------------------------------
#
# Project created by QtCreator 2014-06-12T02:26:50
#
#-------------------------------------------------

QT       -= gui

TARGET = isDigitStr
TEMPLATE = lib

DEFINES += ISDIGITSTR_LIBRARY

SOURCES += isdigitstr.cpp

HEADERS += isdigitstr.h\
        isdigitstr_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
