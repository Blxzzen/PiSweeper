# PiSweeper.pro

TEMPLATE = app
TARGET = pisweeper
INCLUDEPATH += .

# Input
SOURCES += main.cpp \
           pisweeper.cpp 

HEADERS += pisweeper.h \
           pisweeperwindow.h 

RESOURCES += resources.qrc

QT += widgets

