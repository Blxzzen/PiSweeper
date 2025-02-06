#ifndef SKINSMENU_H
#define SKINSMENU_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QPixmap>

// Declarations 
class SkinsMenu : public QWidget {
    Q_OBJECT
public:
    explicit SkinsMenu(QWidget *parent = nullptr);

signals:
    void skinSelected(QString skinName);
    void backToMenu(); // Return to main menu

private slots:
    void selectSkin(QString skinName);

private:
    void loadSkins();
    QVBoxLayout *layout;
};

#endif // SKINSMENU_H