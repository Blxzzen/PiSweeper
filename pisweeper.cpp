#include "pisweeper.h"
#include <QPixmap>
#include <QIcon>
#include <QPushButton>
#include <QGridLayout>

PiSweeper::PiSweeper(QWidget *parent) : QWidget(parent) {
    gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0,0,0,0);
    gridLayout->setHorizontalSpacing(0);
    gridLayout->setVerticalSpacing(0);

    setupBoard();
}

void PiSweeper::setupBoard() {
    QPixmap pixmap(":/images/default.jpg");
    pixmap = pixmap.scaled(20, 20, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    for (int row = 0; row < rows; ++row) {
        QVector<QPushButton*> rowButtons;
        for (int col = 0; col < cols; ++col) {
            QPushButton *button = new QPushButton;

            // Prevent resizing issues
            button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            button->setMinimumSize(20, 20);
            button->setMaximumSize(20, 20);
            button->setFixedSize(20, 20);
            
            // Remove ALL internal spacing or borders
            button->setStyleSheet("QPushButton {"
                                  " padding: 0px; "
                                  " margin: 0px; "
                                  " border: none; "
                                  " outline: none; "
                                  " background: none; "
                                  " }");

            button->setIcon(QIcon(pixmap));
            button->setIconSize(pixmap.size());
            
            gridLayout->addWidget(button, row, col);
            connect(button, &QPushButton::clicked, this, &PiSweeper::buttonClicked);
            rowButtons.push_back(button);
        }
        buttons.push_back(rowButtons);
    }

    // Force the grid to be FIXED and not distribute extra space
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setSizeConstraint(QLayout::SetFixedSize);

    for (int i = 0; i < rows; ++i)
        gridLayout->setRowStretch(i, 0);
    
    for (int i = 0; i < cols; ++i)
        gridLayout->setColumnStretch(i, 0);
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
