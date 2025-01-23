#include "pisweeper.h"
#include <QPushButton>
#include <QGridLayout>

PiSweeper::PiSweeper(QWidget *parent) : QWidget(parent) {
    gridLayout = new QGridLayout(this);
    setupBoard();
}

PiSweeper::~PiSweeper() {}

void PiSweeper::setupBoard() {
    for (int row = 0; row < rows; ++row) {
        QVector<QPushButton*> rowButtons;
        for (int col = 0; col < cols; ++col) {
            QPushButton *button = new QPushButton;
            button->setFixedSize(20, 20);
            gridLayout->addWidget(button, row, col);
            connect(button, &QPushButton::clicked, this, &PiSweeper::buttonClicked);
            rowButtons.push_back(button);
        }
        buttons.push_back(rowButtons);
    }
}

void PiSweeper::buttonClicked() {
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (clickedButton) {
        // Placeholder for game logic
        clickedButton->setText("Clicked");
    }
}
