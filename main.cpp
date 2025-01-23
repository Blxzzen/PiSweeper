#include <QApplication>
#include "pisweeper.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    PiSweeper window;
    window.show();
    return app.exec();
}
