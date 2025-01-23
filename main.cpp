#include <QApplication>
#include "pisweeperwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    PiSweeperWindow window;
    window.show();
    return app.exec();
}
