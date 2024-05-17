QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    coin.cpp \
    enemy.cpp \
    itemshopwindow.cpp \
    level1.cpp \
    level2.cpp \
    level3.cpp \
    level4.cpp \
    level_base.cpp \
    main.cpp \
    mainwindow.cpp \
    platform.cpp \
    player.cpp

HEADERS += \
    coin.h \
    enemy.h \
    itemshopwindow.h \
    level1.h \
    level2.h \
    level3.h \
    level4.h \
    level_base.h \
    mainwindow.h \
    platform.h \
    player.h

FORMS += \
    itemshopwindow.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES +=
