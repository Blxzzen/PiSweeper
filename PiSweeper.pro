# PiSweeper.pro

TEMPLATE = app
TARGET = pisweeper
INCLUDEPATH += .

# Input
SOURCES += main.cpp \
           pisweeper.cpp \
           gameover.cpp

HEADERS += pisweeper.h \
           pisweeperwindow.h \
           gameover.h

RESOURCES += resources.qrc

QT += widgets

