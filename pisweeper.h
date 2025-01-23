#ifndef PISWEEPER_H
#define PISWEEPER_H

#include <QWidget>
#include <QVector>

class QPushButton;
class QGridLayout;

class PiSweeper : public QWidget {
    Q_OBJECT
public:
    explicit PiSweeper(QWidget *parent = nullptr);
    ~PiSweeper();

private:
    QGridLayout *gridLayout;
    QVector<QVector<QPushButton*>> buttons;
    int rows = 16;
    int cols = 30;

    void setupBoard();
    void buttonClicked();
};

#endif // PISWEEPER_H
