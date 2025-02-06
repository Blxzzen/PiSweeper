#include "gameover.h"

// Systematically reveals all bombs on the board and make nothing clickable
void GameOverHandler::revealAllBombs(QVector<QVector<QPushButton*>>& buttons, QVector<QVector<bool>>& bombs, const QString &skin) {
    for (int row = 0; row < buttons.size(); ++row) {
        for (int col = 0; col < buttons[row].size(); ++col) {
            if (bombs[row][col]) {
                QPixmap bombPixmap(":/images/" + skin + "/bomb.jpg"); // Bomb image based on skin
                bombPixmap = bombPixmap.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation); // Size image
                buttons[row][col]->setIcon(QIcon(bombPixmap)); 
                buttons[row][col]->setIconSize(bombPixmap.size()); 
            }
            buttons[row][col]->setAttribute(Qt::WA_TransparentForMouseEvents, true); // Make buttons not clickable
            buttons[row][col]->setFocusPolicy(Qt::NoFocus);  // Prevent the built in button highlight

        }
    }
}

// Show the game over overlay
void GameOverHandler::showGameOver(QWidget* parent) {
    QLabel *gameOverLabel = new QLabel("Game Over", parent);
    gameOverLabel->setFont(QFont("DejaVu Sans", 36, QFont::Bold));
    gameOverLabel->setStyleSheet("color: red; background: rgba(0, 0, 0, 150); padding: 10px;");
    gameOverLabel->setAlignment(Qt::AlignCenter);

    gameOverLabel->setGeometry(parent->width() / 2 - 200, parent->height() / 2 - 50, 400, 100); 
    gameOverLabel->show();
}
