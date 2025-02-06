#ifndef PISWEEPER_H
#define PISWEEPER_H

#include "gameover.h"
#include "victory.h"
#include <QMainWindow> 
#include <QVector>
#include <QPushButton>
#include <QGridLayout>

// Decalrations
class PiSweeper : public QWidget { 
    Q_OBJECT
public:
    explicit PiSweeper(QWidget *parent = nullptr, const QString &skinName = "default");

    QSize sizeHint() const;

private:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QVector<QVector<QPushButton*>> buttons;
    QVector<QVector<bool>> bombs;
    QVector<QVector<bool>> flags;
    int rows = 16; // Grid rows
    int cols = 30; // Grid columns
    int totalBombs = 99; // Amount of bombs in grid
    bool gameOver = false; // Tracks game status
    bool firstClick = true; // Track whether first click of the game or not
    QString skin; // Passes skin through

    // Declared functions + their parameters
    void setupBoard();
    void placeBombs(int firstRow, int firstCol);
    void buttonClicked();
    int countBombs(int x, int y);

    void mousePressEvent(QMouseEvent *event);
    void rightClickHandler(QPushButton *button);
    void revealAdjacentEmptyTiles(int x, int y); 
    void setNumberedTileAppearance(QPushButton *button, int bombCount);
    bool checkVictory();
    void updateWins();
};

#endif // PISWEEPER_H
