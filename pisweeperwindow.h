#ifndef PISWEEPERWINDOW_H
#define PISWEEPERWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QToolBar>
#include <QMouseEvent>
#include "pisweeper.h"
#include "menu.h"
#include "skinsmenu.h"

class PiSweeperWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit PiSweeperWindow(QWidget *parent = nullptr);

private slots:
    void startGame();  // Function to switch from Menu to Game
    void openSkinsMenu();
    void applySkin(QString skinName);

private:
    QWidget *mainWidget;
    QVBoxLayout *mainLayout;
    QToolBar *toolBar;
    Menu *menu;
    PiSweeper *game;
    SkinsMenu *skinsMenu;
    QString currentSkin;

    void setupToolBar();
    bool eventFilter(QObject *obj, QEvent *event) override;

    void returnToMenu();

    QPoint lastMousePosition;
    bool dragging = false;
};

#endif // PISWEEPERWINDOW_H
