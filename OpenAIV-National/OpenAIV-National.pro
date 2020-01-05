QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Datatypes/hierarchy.cpp \
    Datatypes/names.cpp \
    Filetypes/hierarchyfile.cpp \
    Filetypes/namesfile.cpp \
    aboutdialog.cpp \
    configuration.cpp \
    logging.cpp \
    main.cpp \
    mainwindow.cpp \
    nationalitem.cpp \
    nationalmodel.cpp

HEADERS += \
    Datatypes/hierarchy.h \
    Datatypes/names.h \
    Filetypes/hierarchyfile.h \
    Filetypes/namesfile.h \
    aboutdialog.h \
    configuration.h \
    logging.h \
    mainwindow.h \
    nationalitem.h \
    nationalmodel.h

FORMS += \
    aboutdialog.ui \
    mainwindow.ui

# Rules for installation
isEmpty(PREFIX) {
    PREFIX = /usr/local
}
unix:!android: target.path = $$PREFIX/bin/
!isEmpty(target.path): INSTALLS += target
