#ifndef PISWEEPER_H
#define PISWEEPER_H

#include <QWidget>
#include <QVector>
#include <QPushButton>
#include <QGridLayout>

class PiSweeper : public QWidget {
    Q_OBJECT
public:
    explicit PiSweeper(QWidget *parent = nullptr);

private:
    QGridLayout *gridLayout;
    QVector<QVector<QPushButton*>> buttons;
    QVector<QVector<bool>> bombs;
    int rows = 16;
    int cols = 30;
    int totalBombs = 99;

    void setupBoard();
    void placeBombs();
    void buttonClicked();
    int countBombs(int x, int y);
};

#endif // PISWEEPER_H
