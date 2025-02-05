#include "pisweeper.h"
#include <QPixmap>
#include <QIcon>
#include <QPushButton>
#include <QGridLayout>
#include <QRandomGenerator>

PiSweeper::PiSweeper(QWidget *parent) : QWidget(parent) {
    
    centralWidget = new QWidget(this);  // Create a central widget
    QWidget *gridContainer = new QWidget(centralWidget);
    gridLayout = new QGridLayout(gridContainer);
    gridContainer->setLayout(gridLayout);

    gridLayout = new QGridLayout(centralWidget);

    setupBoard();
    placeBombs(); // 🔥 Place bombs after setting up board

    centralWidget->setLayout(gridLayout);
    update();
    show();
}

QSize PiSweeper::sizeHint() const {
    return QSize(998, 580); // Enough space for 30 columns, 16 rows
}

void PiSweeper::setupBoard() {
    QPixmap pixmap(":/images/default/default.jpg");
    pixmap = pixmap.scaled(32, 32);

    // 🔥 Properly resize and initialize bomb grid
    bombs.resize(rows);
    for (int i = 0; i < rows; ++i) {
        bombs[i].resize(cols);
        std::fill(bombs[i].begin(), bombs[i].end(), false); // Explicitly initialize all to false
    }

    for (int row = 0; row < rows; ++row) {
        QVector<QPushButton*> rowButtons;
        for (int col = 0; col < cols; ++col) {
            QPushButton *button = new QPushButton;

            button->setMinimumSize(32, 32);
            button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            button->setFixedSize(32, 32);
            
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

    gridLayout->setSpacing(0);
}


// 🔥 Places bombs at random positions
void PiSweeper::placeBombs() {
    int placedBombs = 0;
    while (placedBombs < totalBombs) {
        int r = QRandomGenerator::global()->bounded(rows);
        int c = QRandomGenerator::global()->bounded(cols);

        if (!bombs[r][c]) {
            bombs[r][c] = true;
            placedBombs++;
        }
    }
}

void PiSweeper::buttonClicked() {
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton) {
        return;
    }

    int index = gridLayout->indexOf(clickedButton);
    if (index == -1) {
        return;
    }

    int row = index / cols;
    int col = index % cols;

    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        return;
    }

    if (bombs[row][col]) {
        QPixmap bombPixmap(":/images/default/defaultbomb.jpg");
        bombPixmap = bombPixmap.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        
        clickedButton->setIcon(QIcon(bombPixmap));
        clickedButton->setIconSize(bombPixmap.size());
    } else {
        int bombCount = countBombs(row, col);
        clickedButton->setText(bombCount > 0 ? QString::number(bombCount) : "");
    }

    clickedButton->setEnabled(false);
}


// 🔥 Counts adjacent bombs
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
