#ifndef VICTORY_H
#define VICTORY_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <QVector>

// Declarations
class VictoryHandler {
public:
    static void revealAllTiles(QVector<QVector<QPushButton*>>& buttons);
    static void showVictory(QWidget* parent);
};

#endif // VICTORY_H
