#include "victory.h"

// Reveal all tiles on win
void VictoryHandler::revealAllTiles(QVector<QVector<QPushButton*>>& buttons) {
    for (int row = 0; row < buttons.size(); ++row) {
        for (int col = 0; col < buttons[row].size(); ++col) {
            buttons[row][col]->setAttribute(Qt::WA_TransparentForMouseEvents, true);
            buttons[row][col]->setFocusPolicy(Qt::NoFocus);  // Prevent focus highlight

        }
    }
}

// Show the victory text and background
void VictoryHandler::showVictory(QWidget* parent) {
    QLabel *victoryLabel = new QLabel("Victory!", parent);
    victoryLabel->setFont(QFont("DejaVu Sans", 36, QFont::Bold));
    victoryLabel->setStyleSheet("color: #52cbff; background: rgba(0, 0, 0, 150); padding: 10px;");
    victoryLabel->setAlignment(Qt::AlignCenter);

    victoryLabel->setGeometry(parent->width() / 2 - 200, parent->height() / 2 - 50, 400, 100);
    victoryLabel->show();
}
