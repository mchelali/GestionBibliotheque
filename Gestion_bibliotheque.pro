#-------------------------------------------------
#
# Project created by QtCreator 2015-11-14T16:24:02
#
#-------------------------------------------------

#ICON = icon.icns
#QMAKE_INFO_PLIST = Info.plist

QMAKE_MAC_SDK = macosx
QT       += core gui printsupport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Gestion_bibliotheque
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    database.cpp \
    addbook.cpp \
    modbook.cpp \
    piechart.cpp \
    adduser.cpp \
    moduser.cpp \
    addauthor.cpp \
    modauthor.cpp \
    empreinter.cpp

HEADERS  += mainwindow.h \
    database.h \
    addbook.h \
    modbook.h \
    piechart.h \
    adduser.h \
    moduser.h \
    addauthor.h \
    modauthor.h \
    empreinter.h

FORMS    += mainwindow.ui \
    addbook.ui \
    modbook.ui \
    adduser.ui \
    moduser.ui \
    addauthor.ui \
    modauthor.ui \
    empreinter.ui

#DEFINES += PROJECT_PATH="\"$PWD"\"
#DEFINES += BUILD_PATH="$OUT_PWD"

RESOURCES += \
    images.qrc

