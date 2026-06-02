#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QLabel>

class Game;
class QGraphicsView;
class QPushButton;
class QHBoxLayout;
class QLabel;
class PlayerInfoPanel;
class ShopPanel;

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget* parent = nullptr);
    ~GameWindow();

private slots:
    void onResetButtonClicked();
    void onFightButtonClicked();
    void onCombatEnded(bool playerWon, int value);
    void onPhaseChanged();

private:
    void setupUI();

    QWidget* m_centralWidget;
    QHBoxLayout* m_mainLayout;
    QGraphicsView* m_view;
    QPushButton* m_resetButton;
    QPushButton* m_fightButton;
    Game* m_game;

private:
    QLabel* m_hpLabel;
    QLabel* m_goldLabel;
    QLabel* m_popLabel;
    QLabel* m_resultLabel;
    PlayerInfoPanel* m_playerPanel;
    ShopPanel* m_shopPanel;

private:
    void refreshPlayerInfo();
private slots:
    void onPlayerStateChanged();
};

#endif // GAMEWINDOW_H
