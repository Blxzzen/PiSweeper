#include "pisweeperwindow.h"
#include <qcoreapplication.h>
#include <QApplication>


PiSweeperWindow::PiSweeperWindow(QWidget *parent) : QMainWindow(parent), game(nullptr) {
    setWindowTitle("PiSweeper");
    resize(998, 580);

    // Main layout container
    mainWidget = new QWidget(this);
    mainLayout = new QVBoxLayout(mainWidget);

    // Create Toolbar
    setupToolBar();

    // Create Menu (Starts as the first screen)
    menu = new Menu(this);
    connect(menu, &Menu::playClicked, this, &PiSweeperWindow::startGame);

    // Add menu to layout
    mainLayout->addWidget(menu);
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
}

void PiSweeperWindow::startGame() {
    // Remove the menu
    delete menu;
    menu = nullptr;

    // Load Minesweeper game
    game = new PiSweeper(this);
    mainLayout->addWidget(game);
}

void PiSweeperWindow::setupToolBar() {
    toolBar = new QToolBar(this);
    addToolBar(Qt::TopToolBarArea, toolBar);

    toolBar->setMovable(false);
    toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    toolBar->setVisible(true);

    // Create Menu Button
    QAction *menuAction = new QAction("Menu", this);
    connect(menuAction, &QAction::triggered, this, &PiSweeperWindow::returnToMenu); 

    // Create Exit Button
    QAction *exitAction = new QAction("Exit", this);
    connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);

    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    toolBar->addAction(menuAction);
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

bool PiSweeperWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj->inherits("QToolBar")) {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                dragging = true;
                lastMousePosition = mouseEvent->globalPos() - frameGeometry().topLeft();
                return true;
            }
        } else if (event->type() == QEvent::MouseMove) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if (dragging && mouseEvent->buttons() & Qt::LeftButton) {
                move(mouseEvent->globalPos() - lastMousePosition);
                return true;
            }
        } else if (event->type() == QEvent::MouseButtonRelease) {
            dragging = false;
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void PiSweeperWindow::returnToMenu() {
    // Remove the game instance if it's active
    if (game) {
        delete game;
        game = nullptr;
    }

    // Show the menu again
    menu = new Menu(this);
    connect(menu, &Menu::playClicked, this, &PiSweeperWindow::startGame);
    mainLayout->addWidget(menu);
}
