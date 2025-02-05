# PiSweeper.pro

TEMPLATE = app
TARGET = pisweeper
INCLUDEPATH += .

# Input
SOURCES += main.cpp \
           pisweeper.cpp \
           pisweeperwindow.cpp \
           gameover.cpp \
           menu.cpp

HEADERS += pisweeper.h \
           pisweeperwindow.h \
           gameover.h \
           menu.h

RESOURCES += resources.qrc

QT += widgets

