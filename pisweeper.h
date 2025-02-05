#ifndef PISWEEPER_H
#define PISWEEPER_H

#include <QMainWindow>  // Change from QWidget to QMainWindow
#include <QVector>
#include <QPushButton>
#include <QGridLayout>

class PiSweeper : public QWidget {  // Inherit from QMainWindow
    Q_OBJECT
public:
    explicit PiSweeper(QWidget *parent = nullptr);

    QSize sizeHint() const;

private:
    QWidget *centralWidget;  // Needed for QMainWindow
    QGridLayout *gridLayout;
    QVector<QVector<QPushButton*>> buttons;
    QVector<QVector<bool>> bombs;
    QVector<QVector<bool>> flags;
    int rows = 16;
    int cols = 30;
    int totalBombs = 99;

    void setupBoard();
    void placeBombs();
    void buttonClicked();
    int countBombs(int x, int y);

    void mousePressEvent(QMouseEvent *event);
    void rightClickHandler(QPushButton *button);
    void revealAdjacentEmptyTiles(int x, int y); 
    void setNumberedTileAppearance(QPushButton *button, int bombCount);
};

#endif // PISWEEPER_H
