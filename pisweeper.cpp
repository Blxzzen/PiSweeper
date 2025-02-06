#include "pisweeper.h"
#include "gameover.h"

#include <QPixmap>
#include <QIcon>
#include <QPushButton>
#include <QGridLayout>
#include <QRandomGenerator>
#include <QVariant>
#include <QFont>
#include <QLabel>
#include <QMouseEvent>
#include <QJsonObject>
#include <QJsonDocument>

// Main game
PiSweeper::PiSweeper(QWidget *parent, const QString &skinName) : QWidget(parent), skin(skinName), firstClick(true) {

    centralWidget = new QWidget(this);
    QWidget *gridContainer = new QWidget(centralWidget);
    gridLayout = new QGridLayout(gridContainer);
    gridContainer->setLayout(gridLayout);

    gridLayout = new QGridLayout(centralWidget);

    setupBoard(); // Sets up the tiles

    centralWidget->setLayout(gridLayout);
    update();
    show();
}

QSize PiSweeper::sizeHint() const {
    return QSize(998, 580); // Setting fixed size, was easier than making responsive
}

// Getting everything ready to play and use
void PiSweeper::setupBoard() {
    QPixmap pixmap(":/images/" + skin + "/block.jpg");  // Use selected skin
    pixmap = pixmap.scaled(32, 32);

    bombs.resize(rows);
    flags.resize(rows);

    for (int i = 0; i < rows; ++i) {
        bombs[i].resize(cols);
        flags[i].resize(cols);
        std::fill(flags[i].begin(), flags[i].end(), false);
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

// Place the bombs at speicifc tile indices
void PiSweeper::placeBombs(int firstRow, int firstCol) {
    int placedBombs = 0;

    while (placedBombs < totalBombs) {
        int r = QRandomGenerator::global()->bounded(rows);
        int c = QRandomGenerator::global()->bounded(cols);

        // Skip first click and adjacent tiles
        if ((r >= firstRow - 1 && r <= firstRow + 1) &&
            (c >= firstCol - 1 && c <= firstCol + 1)) {
            continue;  // Skip placing bombs near first click
        }

        if (!bombs[r][c]) {
            bombs[r][c] = true;
            placedBombs++;
        }
    }
}


// Run appropriate checks each time a button is clicked
void PiSweeper::buttonClicked() {
    if (gameOver) return; // if the game is already over, do nothing

    
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton) return; // Ensures button validity

    int index = gridLayout->indexOf(clickedButton);
    if (index == -1) return; // Ensures button index is in grid layout

    int row = index / cols;
    int col = index % cols;

    if (row < 0 || row >= rows || col < 0 || col >= cols) return; // Ensures rows/cols in valid bounds

    if (flags[row][col] || clickedButton->property("flagged").toBool()) {
        return;  // Don't allow clicking flagged tiles
    }

    // First click of game: Place bombs dynamically avoiding this tile
    if (firstClick) {
        placeBombs(row, col);  // Place bombs but exclude first click via row and col index
        firstClick = false; // Turn first click off for the rest of current game
    }

    if (bombs[row][col]) {
        QPixmap bombPixmap(":/images/" + skin + "/bomb.jpg");  // Load bomb image from the correct skin
        bombPixmap = bombPixmap.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        clickedButton->setIcon(QIcon(bombPixmap));
        clickedButton->setIconSize(bombPixmap.size());
        GameOverHandler::revealAllBombs(buttons, bombs, skin);
        GameOverHandler::showGameOver(this);
        gameOver = true;
        return;
    } else {
        int bombCount = countBombs(row, col);

        if (bombCount > 0) {
            setNumberedTileAppearance(clickedButton, bombCount);
            clickedButton->setProperty("revealed", QVariant(true));  // Mark as revealed
            
            if (checkVictory()) {
                VictoryHandler::revealAllTiles(buttons);
                VictoryHandler::showVictory(this);
                updateWins();  // Track wins in JSON
                gameOver = true;
            }
            
        } else {
            QPixmap emptyPixmap(":/images/" + skin + "/empty.jpg");  // Load empty image from the correct skin
            emptyPixmap = emptyPixmap.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            clickedButton->setIcon(QIcon(emptyPixmap));
            clickedButton->setIconSize(emptyPixmap.size());

            revealAdjacentEmptyTiles(row, col);
        }
    }

    clickedButton->setAttribute(Qt::WA_TransparentForMouseEvents, true);
}

// Counts the bombs around a given tile (most likely tile just pressed unless adjacent tiles are empty)
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

// Hnadles the difference between right and left clicks
void PiSweeper::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        QPushButton *clickedButton = qobject_cast<QPushButton*>(childAt(event->pos()));
        if (clickedButton) {
            rightClickHandler(clickedButton);
        }
    } else {
        QWidget::mousePressEvent(event);
    }
}

// Handles specifically right clicks
void PiSweeper::rightClickHandler(QPushButton *button) {
    int index = gridLayout->indexOf(button);
    if (index == -1) return;

    int row = index / cols;
    int col = index % cols;

    if (row < 0 || row >= rows || col < 0 || col >= cols) return;

    if (!flags[row][col]) {
        // First right-click: Set flag
        QPixmap flagPixmap(":/images/" + skin + "/flag.jpg");  // Load flag image from the correct skin
        flagPixmap = flagPixmap.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        button->setIcon(QIcon(flagPixmap));
        button->setIconSize(flagPixmap.size());

        flags[row][col] = true;
        button->setProperty("flagged", true);  // Prevent left-clicking
        button->setProperty("state", 1);  // Track state (flag)

    } else if (button->property("state").toInt() == 1) {
        // Second right-click: Set question mark
        QPixmap questionPixmap(":/images/" + skin + "/question.jpg");  // Load question image from the correct skin
        questionPixmap = questionPixmap.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        button->setIcon(QIcon(questionPixmap));
        button->setIconSize(questionPixmap.size());

        button->setProperty("state", 2);  // Track state (question mark)
        
    } else {
        // Third right-click: Reset tile
        QPixmap defaultPixmap(":/images/" + skin + "/block.jpg");  // Load block image from the correct skin
        defaultPixmap = defaultPixmap.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        button->setIcon(QIcon(defaultPixmap));
        button->setIconSize(defaultPixmap.size());

        flags[row][col] = false;
        button->setProperty("flagged", false);  // Allow left-clicking again
        button->setProperty("state", 0);  // Reset state
    }
}


// Reveal empty tiles when multiple are revealed
void PiSweeper::revealAdjacentEmptyTiles(int x, int y) {
    if (x < 0 || x >= rows || y < 0 || y >= cols) return;

    QPushButton *button = buttons[x][y];
    if (!button || button->property("revealed").toBool()) return;  // Already revealed

    button->setProperty("revealed", QVariant(true));  // Ensure the tile is marked as revealed

    int bombCount = countBombs(x, y);

    // If bombs are nearby stop current recursive loop and mark them
    if (bombCount > 0) {
        setNumberedTileAppearance(button, bombCount);
        return;
    }

    QPixmap emptyPixmap(":/images/" + skin + "/empty.jpg");  // Load empty image from the correct skin
    emptyPixmap = emptyPixmap.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    button->setIcon(QIcon(emptyPixmap));
    button->setIconSize(emptyPixmap.size());

    button->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    button->setFocusPolicy(Qt::NoFocus);

    // Recursive reveal while skipping current tile
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;
            revealAdjacentEmptyTiles(x + dx, y + dy);
        }
    }

    // Check whether the user has won or not
    if (checkVictory()) {
        VictoryHandler::revealAllTiles(buttons);
        VictoryHandler::showVictory(this);
        updateWins();  // Track wins in JSON for skin unlocks
        gameOver = true;
    }
}


// Set tile number if theres a bomb nearby
void PiSweeper::setNumberedTileAppearance(QPushButton *button, int bombCount) {
    // Set empty tile background
    QPixmap emptyPixmap(":/images/" + skin + "/empty.jpg");  // Load empty image from the correct skin
    emptyPixmap = emptyPixmap.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    button->setIcon(QIcon(emptyPixmap));
    button->setIconSize(emptyPixmap.size());

    // Remove previous QLabel (if any) to prevent overlap, (shouldn't need to do this but just in case)
    QLabel *existingLabel = button->findChild<QLabel*>();
    if (existingLabel) {
        delete existingLabel;
    }

    // Create a new QLabel for number overlay
    QLabel *label = new QLabel(button);
    label->setText(QString::number(bombCount));
    label->setFont(QFont("DejaVu Sans", 18, QFont::Bold));
    label->setAlignment(Qt::AlignCenter);
    
    // Color code numbers based on amount of mines touching tile
    QString color;
    switch (bombCount) {
        case 1: color = "#52cbff"; break;  // Light Blue
        case 2: color = "#52ff80"; break;  // Light Green
        case 3: color = "#ff3b3b"; break;  // Light Red
        case 4: color = "#ff78cb"; break;  // Pink
        case 5: color = "#ff8b2b"; break;  // Light Orange
        case 6: color = "#a333ff"; break;  // Purple
        case 7: color = "#ffe552"; break;  // Light Yellow
        case 8: color = "#ffffff"; break;  // White
    }
    
    label->setStyleSheet("color: " + color + "; font-weight: bold; font-size: 18px;");

    // Ensure the label is centered inside the button
    label->setGeometry(button->rect()); 

    label->show();

    if (checkVictory()) {
        VictoryHandler::revealAllTiles(buttons);
        VictoryHandler::showVictory(this);
        updateWins();  // Track wins in JSON for skins unlock
        gameOver = true;
    }

}

// Check if the user has won the game
bool PiSweeper::checkVictory() {
    int unrevealedTiles = 0;

    // Make sure all tiles without bombs are revealed
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (!bombs[row][col] && !buttons[row][col]->property("revealed").toBool()) {
                unrevealedTiles++;
            }
        }
    }

    return (unrevealedTiles == 0);  // Victory triggers when all nonbomb tiles are revealed
}

// Update win count json data for skin unlocks
void PiSweeper::updateWins() {
    QFile file("skins.json");
    if (!file.open(QIODevice::ReadWrite)) {
        return;
    }

    QByteArray jsonData = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    QJsonObject root = doc.object();

    // Increment the win count
    int wins = root["wins"].toInt();
    wins++;
    root["wins"] = wins;
    qDebug() << "New win count:" << wins;

    // Unlock skins based on wins
    QJsonObject skins = root["skins"].toObject();
    for (QString key : skins.keys()) {
        QJsonObject skinData = skins[key].toObject();
        if (skinData.contains("unlockRequirement")) {
            int requiredWins = skinData["unlockRequirement"].toInt();
            if (wins >= requiredWins && !skinData["unlocked"].toBool()) {
                skinData["unlocked"] = true;
                qDebug() << "Unlocked skin:" << key;
            }
        }
        skins[key] = skinData;
    }
    root["skins"] = skins;

    // Save the updated JSON
    file.resize(0);
    file.write(QJsonDocument(root).toJson());
    file.close();
}
