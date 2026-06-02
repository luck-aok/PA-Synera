#ifndef SHOPPANEL_H
#define SHOPPANEL_H

#include <QWidget>

class QLabel;
class QPushButton;
class QVBoxLayout;
class Player;
class Game;

class ShopPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ShopPanel(Player* player, Game* game, QWidget* parent = nullptr);

protected:
    void showEvent(QShowEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    //void mousePressEvent(QMouseEvent* event) override;

private:
    Player* m_player;
    Game* m_game;

    QLabel* m_titleLabel;
    QPushButton* m_closeButton;
    QWidget* m_card;
};

#endif // SHOPPANEL_H
