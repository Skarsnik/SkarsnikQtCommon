QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

include(../Logs/sqlogging.pri)
include(../SQApplication/sqapplication.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    exampleapp.cpp

HEADERS += \
    exampleapp.h

FORMS += \
    exampleapp.ui

DISTFILES += \
    sqproject.json

TARGET = SQExampleApp
