#ifndef PISWEEPERWINDOW_H
#define PISWEEPERWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QMenu>
#include <QAction>
#include <QApplication> 
#include "pisweeper.h"
#include <QToolBar>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QMenuBar>

class PiSweeperWindow : public QMainWindow {
    Q_OBJECT
public:
    PiSweeperWindow() {
        setWindowTitle("PiSweeper");
        setupToolBar();
        resize(998,580);

        QWidget *mainWidget = new QWidget(this);
        PiSweeper *gameWidget = new PiSweeper(this);

        // Create a vertical layout
        QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);

        // Option A: Use alignment flags
        mainLayout->addWidget(gameWidget, /*stretch=*/0, Qt::AlignCenter);



        mainWidget->setLayout(mainLayout);
        setCentralWidget(mainWidget);

    }

private:
    void setupToolBar() {
        QToolBar *toolBar = new QToolBar(this);
        addToolBar(Qt::TopToolBarArea, toolBar);

        toolBar->setMovable(false);
        toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
        toolBar->setVisible(true);

        QAction *exitAction = new QAction("Exit", this);
        connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);
        
        QWidget *spacer = new QWidget();
        spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        
        toolBar->addWidget(spacer);
        toolBar->addAction(exitAction);

        toolBar->installEventFilter(this); 

        toolBar->setStyleSheet(R"(
            QToolBar {
                background-color: #1e1e1e;
                border-bottom: 2px solid #333;
            }
            QToolBar QToolButton {
                background-color: transparent;
                color: #ffffff;
                padding: 5px 10px;
                border-radius: 4px;
            }
            QToolBar QToolButton:hover {
                background-color: #333;
            }
            QToolBar QToolButton:pressed {
                background-color: #444;
            }
        )");
    }

    QPoint lastMousePosition;
    bool dragging = false;


protected:
    bool eventFilter(QObject *obj, QEvent *event) override {
        if (obj->inherits("QToolBar")) {  // Only track toolbar events
            if (event->type() == QEvent::MouseButtonPress) {
                QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
                if (mouseEvent->button() == Qt::LeftButton) {
                    dragging = true;
                    lastMousePosition = mouseEvent->globalPos() - frameGeometry().topLeft();
                    return true;
                }
            } 
            else if (event->type() == QEvent::MouseMove) {
                QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
                if (dragging && mouseEvent->buttons() & Qt::LeftButton) {
                    move(mouseEvent->globalPos() - lastMousePosition);
                    return true;
                }
            } 
            else if (event->type() == QEvent::MouseButtonRelease) {
                dragging = false;
                return true;
            }
        }
        return QMainWindow::eventFilter(obj, event);
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
