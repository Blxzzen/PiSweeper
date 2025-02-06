#include "skinsmenu.h"
#include <QJsonArray> 

SkinsMenu::SkinsMenu(QWidget *parent) : QWidget(parent) {
    layout = new QVBoxLayout(this);
    QLabel *title = new QLabel("Select a Skin", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 24px; font-weight: bold; color: white;");

    layout->addWidget(title);
    loadSkins();  // Load skins from JSON

    // Back button
    QPushButton *backButton = new QPushButton("Back to Menu", this);
    backButton->setStyleSheet("background-color: #555; color: white; padding: 10px; font-size: 16px;");
    connect(backButton, &QPushButton::clicked, this, &SkinsMenu::backToMenu);
    layout->addWidget(backButton);
}

void SkinsMenu::loadSkins() {
    QFile file("skins.json");
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    QJsonObject root = doc.object();

    if (!root.contains("skins")) {
        return;
    }

    QJsonObject skinsObject = root["skins"].toObject();

    QString currentSkin = root["currentSkin"].toString();

    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setSpacing(10);

    QStringList orderedKeysList;
    for (auto it = skinsObject.begin(); it != skinsObject.end(); ++it) {
        orderedKeysList.append(it.key());
    }

    int row = 0, col = 0;
    for (const QString &key : orderedKeysList) {
        if (!skinsObject.contains(key)) {
            continue;
        }

        QJsonObject skinData = skinsObject[key].toObject();
        bool unlocked = skinData["unlocked"].toBool();
        QString folder = skinData["folder"].toString();

        // Check if folder path is valid
        if (folder.isEmpty()) {
            continue;
        }

        // Load the preview image
        QString imagePath = ":/" + folder + "/block.jpg";
        QPixmap previewPixmap(imagePath);

        if (previewPixmap.isNull()) {
            continue;  // Skip this skin instead of crashing
        }

        previewPixmap = previewPixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // Determine border color (red locked, blue unlocked, green equipped)
        QString borderColor;
        if (key == currentSkin) {
            borderColor = "border: 3px solid #6aff6a;"; // Green (Equipped)
        } else if (unlocked) {
            borderColor = "border: 3px solid #4bd3ff;"; // Blue (Unlocked)
        } else {
            borderColor = "border: 3px solid #ff003c;"; // Red (Locked)
        }

        // Create skin widget
        QWidget *skinWidget = new QWidget(this);
        skinWidget->setStyleSheet("background-color: #222; " + borderColor);
        skinWidget->setFixedSize(300, 120);

        QHBoxLayout *skinLayout = new QHBoxLayout(skinWidget);
        skinLayout->setContentsMargins(10, 10, 10, 10);
        skinLayout->setAlignment(Qt::AlignCenter);

        // Image preview
        QLabel *imageLabel = new QLabel(this);
        imageLabel->setPixmap(previewPixmap);
        imageLabel->setFixedSize(80, 80);
        imageLabel->setAlignment(Qt::AlignCenter);
        imageLabel->setStyleSheet("border: none;");

        // Skin name
        QLabel *nameLabel = new QLabel(key, this);
        nameLabel->setAlignment(Qt::AlignCenter);
        nameLabel->setStyleSheet("color: white; font-size: 16px; font-weight: bold; border: none;");

        // Select button
        QPushButton *selectButton = new QPushButton(this);
        selectButton->setFixedSize(100, 35);
        selectButton->setText(unlocked ? "Select" : "Locked");
        selectButton->setEnabled(unlocked);
        selectButton->setStyleSheet(unlocked
            ? "background-color: #007bff; color: white; font-size: 14px; border-radius: 5px; border: none;"
            : "background-color: #444; color: gray; font-size: 14px; border-radius: 5px; border: none;");

        connect(selectButton, &QPushButton::clicked, this, [this, key]() { selectSkin(key); });

        // Add to layout
        skinLayout->addWidget(imageLabel);
        skinLayout->addWidget(nameLabel);
        skinLayout->addWidget(selectButton);

        skinWidget->setLayout(skinLayout);
        gridLayout->addWidget(skinWidget, row, col);

        col++;
        if (col == 2) {
            col = 0;
            row++;
        }
    }

    layout->addLayout(gridLayout);
}



void SkinsMenu::selectSkin(QString skinName) {
    QFile file("skins.json");
    if (!file.open(QIODevice::ReadWrite)) {
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    QJsonObject root = doc.object();
    
    // Preserve the original order
    QJsonObject skinsObject = root["skins"].toObject();
    QJsonObject newRoot;
    newRoot["currentSkin"] = skinName;
    newRoot["wins"] = root["wins"];
    
    QJsonObject newSkins;
    for (auto it = skinsObject.begin(); it != skinsObject.end(); ++it) {
        newSkins[it.key()] = it.value(); // Keep order
    }
    newRoot["skins"] = newSkins;

    file.open(QIODevice::WriteOnly);
    file.write(QJsonDocument(newRoot).toJson());
    file.close();

    emit skinSelected(skinName);  // Notify PiSweeperWindow
}

