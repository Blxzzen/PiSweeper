#include "menu.h"

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
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, 
                                        stop:0 #00c853, stop:1 #b2ff59); /* Dark Green to Light Green */
            color: white;
            font-size: 16px;
            padding: 10px;
            border-radius: 5px;
        }
        QPushButton:hover {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, 
                                        stop:0 #00e676, stop:1 #ccff90); /* Brighter Green on Hover */
        }
        QPushButton:pressed {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, 
                                        stop:0 #009624, stop:1 #76ff03); /* Slightly Darker Green on Press */
        }
    )");
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
