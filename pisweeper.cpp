#include "pisweeper.h"
#include <QPixmap>
#include <QIcon>
#include <QPushButton>
#include <QGridLayout>
#include <QRandomGenerator>
#include <QVariant>
#include <QFont>
#include <QLabel>

PiSweeper::PiSweeper(QWidget *parent) : QWidget(parent) {
    
    centralWidget = new QWidget(this);
    QWidget *gridContainer = new QWidget(centralWidget);
    gridLayout = new QGridLayout(gridContainer);
    gridContainer->setLayout(gridLayout);

    gridLayout = new QGridLayout(centralWidget);

    setupBoard();
    placeBombs();

    centralWidget->setLayout(gridLayout);
    update();
    show();
}

QSize PiSweeper::sizeHint() const {
    return QSize(998, 580);
}

void PiSweeper::setupBoard() {
    QPixmap pixmap(":/images/default/default.jpg");
    pixmap = pixmap.scaled(32, 32);

    bombs.resize(rows);
    for (int i = 0; i < rows; ++i) {
        bombs[i].resize(cols);
        std::fill(bombs[i].begin(), bombs[i].end(), false);
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
    if (!clickedButton) return;

    int index = gridLayout->indexOf(clickedButton);
    if (index == -1) return;

    int row = index / cols;
    int col = index % cols;

    if (row < 0 || row >= rows || col < 0 || col >= cols) return;

    if (bombs[row][col]) {
        QPixmap bombPixmap(":/images/default/defaultbomb.jpg");
        bombPixmap = bombPixmap.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        clickedButton->setIcon(QIcon(bombPixmap));
        clickedButton->setIconSize(bombPixmap.size());
    } else {
        int bombCount = countBombs(row, col);

        if (bombCount > 0) {
            setNumberedTileAppearance(clickedButton, bombCount);
        } else {
            QPixmap emptyPixmap(":/images/default/defaultempty.jpg");
            emptyPixmap = emptyPixmap.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            clickedButton->setIcon(QIcon(emptyPixmap));
            clickedButton->setIconSize(emptyPixmap.size());

            revealAdjacentEmptyTiles(row, col);
        }
    }

    clickedButton->setAttribute(Qt::WA_TransparentForMouseEvents, true);
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

void PiSweeper::revealAdjacentEmptyTiles(int x, int y) {
    if (x < 0 || x >= rows || y < 0 || y >= cols) return;

    QPushButton *button = buttons[x][y];
    if (!button || !button->isEnabled()) return;

    if (button->property("revealed").toBool()) return;

    button->setProperty("revealed", QVariant(true));

    int bombCount = countBombs(x, y);

    if (bombCount > 0) {
        setNumberedTileAppearance(button, bombCount);
        return;
    }

    QPixmap emptyPixmap(":/images/default/defaultempty.jpg");
    emptyPixmap = emptyPixmap.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    button->setIcon(QIcon(emptyPixmap));
    button->setIconSize(emptyPixmap.size());

    button->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    button->setFocusPolicy(Qt::NoFocus);

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;
            revealAdjacentEmptyTiles(x + dx, y + dy);
        }
    }
}


void PiSweeper::setNumberedTileAppearance(QPushButton *button, int bombCount) {
    // Set empty tile background
    QPixmap emptyPixmap(":/images/default/defaultempty.jpg");
    emptyPixmap = emptyPixmap.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    button->setIcon(QIcon(emptyPixmap));
    button->setIconSize(emptyPixmap.size());

    // Remove previous QLabel (if any) to prevent overlap
    QLabel *existingLabel = button->findChild<QLabel*>();
    if (existingLabel) {
        delete existingLabel;
    }

    // Create a new QLabel for number overlay
    QLabel *label = new QLabel(button);
    label->setText(QString::number(bombCount));
    label->setFont(QFont("DejaVu Sans", 18, QFont::Bold));
    label->setAlignment(Qt::AlignCenter);
    
    // Color code numbers (like real Minesweeper)
    QString color;
    switch (bombCount) {
        case 1: color = "#0000FF"; break;  // Blue
        case 2: color = "#008000"; break;  // Green
        case 3: color = "#FF0000"; break;  // Red
        case 4: color = "#000080"; break;  // Dark Blue
        case 5: color = "#800000"; break;  // Brown
        case 6: color = "#00FFFF"; break;  // Cyan
        case 7: color = "#000000"; break;  // Black
        case 8: color = "#808080"; break;  // Gray
    }
    
    label->setStyleSheet("color: " + color + "; font-weight: bold; font-size: 18px;");

    // Ensure the label is centered inside the button
    label->setGeometry(button->rect()); 

    label->show();
}


