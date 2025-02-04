#include <QApplication>
#include "pisweeperwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    PiSweeperWindow window;  // Launch PiSweeperWindow (which is the ACTUAL main window)
    window.show();
    return app.exec();
}

