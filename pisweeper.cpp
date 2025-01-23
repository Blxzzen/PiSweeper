#include "pisweeper.h"
#include <QPixmap>
#include <QIcon>
#include <QPushButton>
#include <QGridLayout>

PiSweeper::PiSweeper(QWidget *parent) : QWidget(parent) {
    gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    setupBoard();
}

void PiSweeper::setupBoard() {
    QPixmap pixmap(":/images/box.jpg");  // Load the image
    pixmap = pixmap.scaled(20, 20, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // Scale it to the desired size

    for (int row = 0; row < rows; ++row) {
        QVector<QPushButton*> rowButtons;
        for (int col = 0; col < cols; ++col) {
            QPushButton *button = new QPushButton;
            button->setIcon(QIcon(pixmap));  // Set the pixmap as an icon
            button->setIconSize(pixmap.size());  // Ensure the icon size matches the pixmap
            button->setFixedSize(20, 20);  // Set the fixed size for buttons
            gridLayout->addWidget(button, row, col);
            connect(button, &QPushButton::clicked, this, &PiSweeper::buttonClicked);
            rowButtons.push_back(button);
        }
        buttons.push_back(rowButtons);
    }
}

void PiSweeper::buttonClicked() {
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    int index = gridLayout->indexOf(clickedButton);
    int row = index / cols;
    int col = index % cols;
    
    // Example logic for clicking on the button
    if (bombs[row][col]) {
        clickedButton->setText("Bomb");
    } else {
        int bombCount = countBombs(row, col);
        clickedButton->setText(bombCount > 0 ? QString::number(bombCount) : "");
    }
    clickedButton->setEnabled(false);
}

int PiSweeper::countBombs(int x, int y) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int nx = x + i, ny = y + j;
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && bombs[nx][ny]) {
                count++;
            }
        }
    }
    return count;
}
