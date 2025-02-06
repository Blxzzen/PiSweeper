#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

// Declarations
class Menu : public QWidget {
    Q_OBJECT
public:
    explicit Menu(QWidget *parent = nullptr);

signals:
    void playClicked();  // Signal for when Play button is clicked
    void skinsClicked(); // Signal for when Skins button is clicked

private:
    QLabel *titleLabel;
    QPushButton *playButton;
    QPushButton *skinsButton;
};

#endif // MENU_H
