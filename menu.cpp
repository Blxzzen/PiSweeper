#include "menu.h"

Menu::Menu(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Title
    titleLabel = new QLabel("PiSweeper", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: white;");

    // Play Button
    playButton = new QPushButton("Play", this);
    playButton->setStyleSheet("background-color: #ff8c00; color: white; font-size: 16px; padding: 10px;");
    connect(playButton, &QPushButton::clicked, this, &Menu::playClicked);

    // Skins Button (currently does nothing)
    skinsButton = new QPushButton("Skins", this);
    skinsButton->setStyleSheet("background-color: #444; color: white; font-size: 16px; padding: 10px;");

    // Add widgets to layout
    layout->addStretch();
    layout->addWidget(titleLabel, 0, Qt::AlignCenter);
    layout->addWidget(playButton, 0, Qt::AlignCenter);
    layout->addWidget(skinsButton, 0, Qt::AlignCenter);
    layout->addStretch();

    setLayout(layout);
}
