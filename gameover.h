#ifndef GAMEOVER_H
#define GAMEOVER_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <QVector>

class GameOverHandler {
public:
    static void revealAllBombs(QVector<QVector<QPushButton*>>& buttons, QVector<QVector<bool>>& bombs, const QString &skin);
    static void showGameOver(QWidget* parent);
};

#endif // GAMEOVER_H
