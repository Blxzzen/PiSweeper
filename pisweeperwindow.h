#ifndef PISWEEPERWINDOW_H
#define PISWEEPERWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QApplication> 
#include "pisweeper.h"

class PiSweeperWindow : public QMainWindow {
    Q_OBJECT
public:
    PiSweeperWindow() {
        setWindowTitle("PiSweeper");
        resize(800, 600);

        PiSweeper *gameWidget = new PiSweeper(this);
        setCentralWidget(gameWidget);

        setupMenuBar();
    }

private:
    void setupMenuBar() {
        QMenuBar *menuBar = new QMenuBar();
        QMenu *fileMenu = menuBar->addMenu("&File");

        QAction *toggleFullscreenAction = new QAction("Toggle Fullscreen", this);
        fileMenu->addAction(toggleFullscreenAction);
        connect(toggleFullscreenAction, &QAction::triggered, this, &PiSweeperWindow::toggleFullscreen);

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
