QT += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 # console
# CONFIG -= app_bundle

TARGET = server

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp \
    client.cpp \
    dbcontroller.cpp \
    dbwindow.cpp \
    field.cpp \
    gamecontroller.cpp \
    mainwindow.cpp \
    server.cpp

HEADERS += \
    client.hpp \
    config.hpp \
    dbcontroller.hpp \
    dbwindow.hpp \
    field.hpp \
    gamecontroller.hpp \
    mainwindow.hpp \
    server.hpp

FORMS += \
    mainwindow.ui \
    dbwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    database.qrc \
    images.qrc

DISTFILES += \
    placements.txt  \
    data.db
