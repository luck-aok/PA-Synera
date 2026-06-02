#include "gamewindow.h"
#include "core/game.h"
#include "playerinfopanel.h"
#include "shop/shoppanel.h"
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

GameWindow::GameWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_centralWidget(new QWidget(this))
    , m_mainLayout(new QHBoxLayout())
    , m_view(new QGraphicsView(this))
    , m_resetButton(new QPushButton("Reset", this))
    , m_fightButton(new QPushButton("Fight",this))
    , m_game(new Game(this))
{
    setupUI();
    m_game->initialize();
}

GameWindow::~GameWindow() = default;

void GameWindow::onResetButtonClicked()
{
    if (m_game) {
        m_game->reset();
    }
}

void GameWindow::onFightButtonClicked()
{
    if (!m_game) return;

    if (m_game->currentPhase() == Game::Phase::Resolve) {
        m_game->nextRound();
    } else if (m_game->currentPhase() == Game::Phase::Prep) {
        m_game->startCombat();
    }//fight段按钮禁用
}

void GameWindow::setupUI()
{
    //
    connect(m_game,
            &Game::playerInfoChanged,
            this,
            &GameWindow::onPlayerStateChanged);

    connect(m_game, &Game::combatEnded,
            this, &GameWindow::onCombatEnded);
    connect(m_game, &Game::phaseChanged,
            this, &GameWindow::onPhaseChanged);

    //
    setCentralWidget(m_centralWidget);
    m_centralWidget->setLayout(m_mainLayout);

    setStyleSheet(R"(
        QMainWindow {
            background-color: #2b2b2b;
        }
        QWidget {
            background-color: #2b2b2b;
            color: #f2f2f2;
        }
        QPushButton {
            background-color: #2f2f2f;
            color: #f2f2f2;
            border: 1px solid #565656;
            border-radius: 4px;
            padding: 6px 14px;
            font-size: 13px;
        }
        QPushButton:hover {
            background-color: #3a3a3a;
        }
        QPushButton:pressed {
            background-color: #242424;
        }
    )");

    m_view->setRenderHint(QPainter::Antialiasing, true);
    m_view->setDragMode(QGraphicsView::NoDrag);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    m_view->setResizeAnchor(QGraphicsView::AnchorViewCenter);
    m_view->setMouseTracking(true);
    m_view->viewport()->setMouseTracking(true);

    // 左侧容器（棋盘+按钮+旧信息栏）
    QWidget* leftContainer = new QWidget(m_centralWidget);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftContainer);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(0);

    leftLayout->addWidget(m_view, 1);

    QWidget* fightBar = new QWidget(this);
    QHBoxLayout* fightLayout = new QHBoxLayout(fightBar);
    fightLayout->setContentsMargins(0,0,0,0);
    fightLayout->addWidget(m_fightButton);
    fightLayout->addStretch();
    leftLayout->addWidget(fightBar);

    connect(m_fightButton,&QPushButton::clicked,
            this, &GameWindow::onFightButtonClicked);

    //
    QWidget* controlBar = new QWidget(this);
    QHBoxLayout* controlLayout = new QHBoxLayout(controlBar);
    controlLayout->setContentsMargins(0, 0, 0, 0);
    controlLayout->addWidget(m_resetButton);
    controlLayout->addStretch();
    leftLayout->addWidget(controlBar);

    connect(m_resetButton, &QPushButton::clicked,
            this, &GameWindow::onResetButtonClicked);

    //
    QWidget* infoBar = new QWidget(this);
    QHBoxLayout* infoLayout = new QHBoxLayout(infoBar);
    infoLayout->setContentsMargins(0, 0, 0, 0);

    m_hpLabel = new QLabel( QString("HP:%1").arg(m_game->player()->Hp()), this);
    m_goldLabel = new QLabel(QString("GOLD:%1").arg(m_game->player()->Gold()), this);
    m_popLabel = new QLabel(
        QString("POP: %1/%2").arg(m_game->player()->PopulationUsed())
                             .arg(m_game->player()->PopulationCap()),
        this);

    m_resultLabel = new QLabel("", this);
    m_resultLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #ffcc00;");

    infoLayout->addWidget(m_hpLabel);
    infoLayout->addWidget(m_goldLabel);
    infoLayout->addWidget(m_popLabel);
    infoLayout->addWidget(m_resultLabel);
    infoLayout->addStretch();

    leftLayout->addWidget(infoBar);

    //
    m_view->setScene(m_game->scene());

    // 右侧玩家面板
    m_playerPanel = new PlayerInfoPanel(m_game->player(), m_game, m_centralWidget);
    m_mainLayout->addWidget(leftContainer);
    m_mainLayout->addWidget(m_playerPanel);

    // 商店覆盖层
    m_shopPanel = new ShopPanel(m_game->player(), m_game, this);
    connect(m_playerPanel, &PlayerInfoPanel::shopRequested, this, [this]() {
        m_shopPanel->show();
        m_shopPanel->raise();
    });
}

void GameWindow::onPlayerStateChanged()
{
    refreshPlayerInfo();
}

void GameWindow::refreshPlayerInfo()
{
    Player* p = m_game->player();
    m_hpLabel->setText(QString("HP:%1").arg(p->Hp()));

    m_goldLabel->setText(QString("Gold:%1").arg(p->Gold()));

    m_popLabel->setText(QString("POP:%1/%2").arg(p->PopulationUsed()).arg
  (p->PopulationCap()));

    m_playerPanel->refresh();
}

void GameWindow::onCombatEnded(bool playerWon, int value)
{
    if (playerWon) {
        m_resultLabel->setText(QString("Victory! +%1 Gold").arg(value));
        m_resultLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #4fc3f7;");
    } else {
        m_resultLabel->setText(QString("Defeat! -%1 HP").arg(value));
        m_resultLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #ef5350;");
    }
}

void GameWindow::onPhaseChanged()
{
    Game::Phase phase = m_game->currentPhase();
    if (phase == Game::Phase::Prep) {
        m_fightButton->setText("Fight");
        m_resultLabel->setText("");
    } else if (phase == Game::Phase::Combat) {
        m_fightButton->setText("Fighting...");
        m_resultLabel->setText("");
    } else if (phase == Game::Phase::Resolve) {
        m_fightButton->setText(QString("Next Round (%1)").arg(m_game->round() + 1));
    }
}
