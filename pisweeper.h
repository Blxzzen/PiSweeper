#ifndef PISWEEPER_H
#define PISWEEPER_H

#include "gameover.h"
#include "victory.h"
#include <QMainWindow>  // Change from QWidget to QMainWindow
#include <QVector>
#include <QPushButton>
#include <QGridLayout>

class PiSweeper : public QWidget {  // Inherit from QMainWindow
    Q_OBJECT
public:
    explicit PiSweeper(QWidget *parent = nullptr, const QString &skinName = "default");

    QSize sizeHint() const;

private:
    QWidget *centralWidget;  // Needed for QMainWindow
    QGridLayout *gridLayout;
    QVector<QVector<QPushButton*>> buttons;
    QVector<QVector<bool>> bombs;
    QVector<QVector<bool>> flags;
    int rows = 16;
    int cols = 30;
    int totalBombs = 1;
    bool gameOver = false; // Track if the game is over
    QString skin;

    void setupBoard();
    void placeBombs();
    void buttonClicked();
    int countBombs(int x, int y);

    void mousePressEvent(QMouseEvent *event);
    void rightClickHandler(QPushButton *button);
    void revealAdjacentEmptyTiles(int x, int y); 
    void setNumberedTileAppearance(QPushButton *button, int bombCount);
    bool checkVictory();
};

#endif // PISWEEPER_H
