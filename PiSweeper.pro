# PiSweeper.pro

TEMPLATE = app
TARGET = pisweeper
INCLUDEPATH += .

# Input
SOURCES += main.cpp \
           pisweeper.cpp \
           pisweeperwindow.cpp \
           gameover.cpp \
           menu.cpp \
           victory.cpp

HEADERS += pisweeper.h \
           pisweeperwindow.h \
           gameover.h \
           menu.h \
           victory.h

RESOURCES += resources.qrc

QT += widgets

