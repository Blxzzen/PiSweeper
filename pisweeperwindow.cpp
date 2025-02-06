#include "pisweeperwindow.h"
#include <qcoreapplication.h>
#include <QApplication>


PiSweeperWindow::PiSweeperWindow(QWidget *parent) : QMainWindow(parent), game(nullptr), skinsMenu(nullptr) {
    setWindowTitle("PiSweeper");
    resize(998, 580);

    // Load current skin from JSON
    QFile file("skins.json");
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray jsonData = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        QJsonObject root = doc.object();
        currentSkin = root["currentSkin"].toString();  // Store it
    } else {
        currentSkin = "default";  // Fallback
    }

    // Main layout container
    mainWidget = new QWidget(this);
    mainLayout = new QVBoxLayout(mainWidget);

    // Create Toolbar
    setupToolBar();

    // Create Menu
    menu = new Menu(this);
    connect(menu, &Menu::playClicked, this, &PiSweeperWindow::startGame);
    connect(menu, &Menu::skinsClicked, this, &PiSweeperWindow::openSkinsMenu);

    mainLayout->addWidget(menu);
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
}


void PiSweeperWindow::startGame() {
    delete menu;
    menu = nullptr;

    game = new PiSweeper(this, currentSkin);
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
    // Remove the game if it's running
    if (game) {
        delete game;
        game = nullptr;
    }

    // Remove skins menu if it's active
    if (skinsMenu) {
        delete skinsMenu;
        skinsMenu = nullptr;
    }

    // Clear the layout before adding the menu back
    QLayoutItem *child;
    while ((child = mainLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    // Recreate the main menu
    menu = new Menu(this);
    connect(menu, &Menu::playClicked, this, &PiSweeperWindow::startGame);
    connect(menu, &Menu::skinsClicked, this, &PiSweeperWindow::openSkinsMenu);

    mainLayout->addWidget(menu);
}

void PiSweeperWindow::openSkinsMenu() {
    // Remove the menu first
    if (menu) {
        delete menu;
        menu = nullptr;
    }

    // Remove the game if it was running
    if (game) {
        delete game;
        game = nullptr;
    }

    // Clear the layout
    QLayoutItem *child;
    while ((child = mainLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    // Create and add the skins menu
    skinsMenu = new SkinsMenu(this);
    connect(skinsMenu, &SkinsMenu::skinSelected, this, &PiSweeperWindow::applySkin);
    connect(skinsMenu, &SkinsMenu::backToMenu, this, &PiSweeperWindow::returnToMenu);

    mainLayout->addWidget(skinsMenu);
}

void PiSweeperWindow::applySkin(QString skinName) {
    // Update `currentSkin` in memory
    currentSkin = skinName;

    // Update `currentSkin` in `skins.json`
    QFile file("skins.json");
    if (!file.open(QIODevice::ReadWrite)) {
        qDebug() << "Failed to open skins.json";
        return;
    }

    QByteArray jsonData = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    QJsonObject root = doc.object();
    
    root["currentSkin"] = skinName;  // Overwrite current skin

    file.resize(0);
    file.write(QJsonDocument(root).toJson());
    file.close();

    // Return to menu
    returnToMenu();
}

