#-------------------------------------------------
#
# Project created by QtCreator 2018-07-01T17:40:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtTsTranslator
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    tsfilereader.cpp \
    tsfilewriter.cpp \
    tsfileinfo.cpp \
    translationcontroller.cpp \
    tsfiletranslator.cpp \
    translationconfig.cpp \
    xmlsyntaxhighlighter.cpp \
    tsfileviewer.cpp \
    translationconfigdialog.cpp \
    codeeditor.cpp \
    tstranslatorutils.cpp

HEADERS  += mainwindow.h \
    tstranslator_global.h \
    tsfilereader.h \
    tsfilewriter.h \
    tsfileinfo.h \
    translationcontroller.h \
    tsfiletranslator.h \
    translationconfig.h \
    xmlsyntaxhighlighter.h \
    tsfileviewer.h \
    translationconfigdialog.h \
    codeeditor.h \
    tstranslatorutils.h

# QNetworkTranslator
INCLUDEPATH += ../../QNetworkTranslator/src/
include(../../QNetworkTranslator/src/networktranslator.pri)

TRANSLATIONS += language/qt_$${TARGET}_zh_CN.ts

RESOURCES += resource/resource.qrc

DISTFILES +=
