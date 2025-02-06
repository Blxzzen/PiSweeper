#include "menu.h"

// Menu components 
Menu::Menu(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Title
    titleLabel = new QLabel("PiSweeper", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: white;");

    // Play Button
    playButton = new QPushButton("Play", this);
    playButton->setStyleSheet(R"(
        QPushButton {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, 
                                        stop:0 #02ed60, stop:1 #00ad45); /* Dark Green to Light Green */
            color: white;
            font-size: 16px;
            padding: 10px;
            border-radius: 5px;
            min-width: 180px; /* Increase button width */
        }
        QPushButton:hover {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, 
                                        stop:0 #1aeb6d, stop:1 #10cc5b); /* Brighter Green on Hover */
        }
        QPushButton:pressed {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, 
                                        stop:0 #02ba4c, stop:1 #006328); /* Slightly Darker Green on Press */
        }
    )");

    connect(playButton, &QPushButton::clicked, this, &Menu::playClicked);

    // Skins Button
    skinsButton = new QPushButton("Skins", this);
    skinsButton->setStyleSheet(R"(
        QPushButton {
            background-color: #ff8c00; /* Orange */
            color: white;
            font-size: 16px;
            padding: 10px;
            border-radius: 5px;
            min-width: 180px; /* Increase button width */
        }
        QPushButton:hover {
            background-color: #ff9514; /* Lighter Orange */
        }
        QPushButton:pressed {
            background-color: #d17300; /* Darker Orange */
        }
    )");

    connect(skinsButton, &QPushButton::clicked, this, &Menu::skinsClicked);

    // Add widgets to layout
    layout->addStretch();
    layout->addWidget(titleLabel, 0, Qt::AlignCenter);
    layout->addWidget(playButton, 0, Qt::AlignCenter);
    layout->addWidget(skinsButton, 0, Qt::AlignCenter);
    layout->addStretch();

    setLayout(layout);
}
