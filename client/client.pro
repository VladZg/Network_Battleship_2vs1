QT += core gui network sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += multimedia

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = client

SOURCES += \
    controller.cpp \
    field.cpp \
    fightshistorywindow.cpp \
    images.cpp \
    main.cpp \
    mainwindow.cpp \
    model.cpp

HEADERS += \
    config.hpp \
    constants.hpp \
    controller.hpp \
    field.hpp \
    fightshistorywindow.h \
    images.hpp \
    mainwindow.hpp \
    model.hpp

# PlaySound utility
HEADERS += util/PlaySound.h
SOURCES += util/PlaySound.cpp

CONFIG( unix ) {
    LIBS += -lasound
    SOURCES += util/PlaySound_nix.cpp
    HEADERS += util/PlaySound_nix.h
}

CONFIG( windows ) {
    SOURCES += util/PlaySound_win.cpp
    HEADERS += util/PlaySound_win.h
}

FORMS += \
    fightshistorywindow.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc \
    sounds.qrc
