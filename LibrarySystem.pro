QT       += core gui widgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    databasemanager.cpp \
    book.cpp \
    user.cpp \
    addbookdialog.cpp \
    adduserdialog.cpp \
    borrowdialog.cpp

HEADERS += \
    mainwindow.h \
    databasemanager.h \
    book.h \
    user.h \
    addbookdialog.h \
    adduserdialog.h \
    borrowdialog.h

FORMS += \
    mainwindow.ui

TARGET = LibrarySystem