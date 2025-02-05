#include "gameover.h"

void GameOverHandler::revealAllBombs(QVector<QVector<QPushButton*>>& buttons, QVector<QVector<bool>>& bombs) {
    for (int row = 0; row < buttons.size(); ++row) {
        for (int col = 0; col < buttons[row].size(); ++col) {
            if (bombs[row][col]) {
                QPixmap bombPixmap(":/images/default/defaultbomb.jpg");
                bombPixmap = bombPixmap.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                buttons[row][col]->setIcon(QIcon(bombPixmap));
                buttons[row][col]->setIconSize(bombPixmap.size());
            }
            buttons[row][col]->setEnabled(false);  // Disable all tiles
        }
    }
}

void GameOverHandler::showGameOver(QWidget* parent) {
    QLabel *gameOverLabel = new QLabel("Game Over", parent);
    gameOverLabel->setFont(QFont("DejaVu Sans", 36, QFont::Bold));
    gameOverLabel->setStyleSheet("color: red; background: rgba(0, 0, 0, 150); padding: 10px;");
    gameOverLabel->setAlignment(Qt::AlignCenter);

    gameOverLabel->setGeometry(parent->width() / 2 - 200, parent->height() / 2 - 50, 400, 100);
    gameOverLabel->show();
}
