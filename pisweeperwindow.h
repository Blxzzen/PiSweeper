#ifndef PISWEEPERWINDOW_H
#define PISWEEPERWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QMenuBar>
#include <QApplication>
#include "pisweeper.h"

class PiSweeperWindow : public QMainWindow {
    Q_OBJECT
public:
    PiSweeperWindow() {
        setWindowTitle("PiSweeper");
        resize(800, 600);

        // Ensure standard window decorations are used
        setWindowFlag(Qt::CustomizeWindowHint, false);

        // Create the game widget and set it as the central widget
        PiSweeper *gameWidget = new PiSweeper(this);
        setCentralWidget(gameWidget);

        // Create Menu Bar
        QMenuBar *menuBar = new QMenuBar(this);
        QMenu *fileMenu = menuBar->addMenu("&File");

        // Add Fullscreen toggle action
        QAction *toggleFullscreenAction = new QAction("Toggle Fullscreen", this);
        fileMenu->addAction(toggleFullscreenAction);
        connect(toggleFullscreenAction, &QAction::triggered, this, &PiSweeperWindow::toggleFullscreen);

        // Add Quit action
        QAction *quitAction = new QAction("&Quit", this);
        fileMenu->addAction(quitAction);
        connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

        setMenuBar(menuBar);
    }

private slots:
    void toggleFullscreen() {
        if (isFullScreen()) {
            showNormal();
        } else {
            showFullScreen();
        }
    }
};

#endif // PISWEEPERWINDOW_H
