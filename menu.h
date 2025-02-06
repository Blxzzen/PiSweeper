#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class Menu : public QWidget {
    Q_OBJECT
public:
    explicit Menu(QWidget *parent = nullptr);

signals:
    void playClicked();  // Signal for when Play is clicked
    void skinsClicked();

private:
    QLabel *titleLabel;
    QPushButton *playButton;
    QPushButton *skinsButton;
};

#endif // MENU_H
