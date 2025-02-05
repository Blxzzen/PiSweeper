#include <QApplication>
#include "pisweeperwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    app.setStyleSheet(R"(
        QMainWindow {
            background-color: #121212;
            color: #ffffff;
        }
    )");

    PiSweeperWindow window;  // Launch PiSweeperWindow (which is the ACTUAL main window)
    window.show();
    return app.exec();
}

