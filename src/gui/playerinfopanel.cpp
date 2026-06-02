#include "playerinfopanel.h"
#include "entity/player.h"
#include "core/game.h"
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QVBoxLayout>

PlayerInfoPanel::PlayerInfoPanel(Player* player, Game* game, QWidget* parent)
    : QWidget(parent)
    , m_player(player)
    , m_game(game)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(8);

    // 标题
    m_titleLabel = new QLabel(QStringLiteral("— 玩家状态 —"), this);
    m_titleLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #e0e0e0;");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_titleLabel);

    // HP
    m_hpLabel = new QLabel(this);
    layout->addWidget(m_hpLabel);

    m_hpBar = new QProgressBar(this);
    m_hpBar->setMaximum(m_player->Hp());
    m_hpBar->setValue(m_player->Hp());
    m_hpBar->setTextVisible(true);
    m_hpBar->setMaximumHeight(14);
    m_hpBar->setStyleSheet(R"(
        QProgressBar {
            border: 1px solid #555;
            border-radius: 3px;
            background: #1e1e1e;
            color: #e0e0e0;
            font-size: 10px;
        }
        QProgressBar::chunk {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                stop:0 #c0392b, stop:0.5 #e74c3c, stop:1 #c0392b);
            border-radius: 2px;
        }
    )");
    layout->addWidget(m_hpBar);

    // Gold
    m_goldLabel = new QLabel(this);
    m_goldLabel->setStyleSheet("color: #f1c40f; font-size: 13px;");
    layout->addWidget(m_goldLabel);

    // Level
    m_levelLabel = new QLabel(this);
    layout->addWidget(m_levelLabel);

    // POP
    m_popLabel = new QLabel(this);
    layout->addWidget(m_popLabel);

    // 轮次
    m_roundLabel = new QLabel(this);
    layout->addWidget(m_roundLabel);

    layout->addStretch();

    // 商店按钮
    m_shopButton = new QPushButton(QStringLiteral("商店"), this);
    connect(m_shopButton, &QPushButton::clicked, this, &PlayerInfoPanel::shopRequested);
    layout->addWidget(m_shopButton);

    setFixedWidth(180);
    setStyleSheet(R"(
        PlayerInfoPanel {
            background-color: #252525;
            border-left: 2px solid #3a3a3a;
        }
    )");
}

void PlayerInfoPanel::refresh()
{
    m_hpLabel->setText(QString("HP: %1 / %2").arg(m_player->Hp()).arg(100));
    m_hpBar->setMaximum(100);
    m_hpBar->setValue(m_player->Hp());

    m_goldLabel->setText(QString("Gold: %1").arg(m_player->Gold()));

    m_levelLabel->setText(QString("Level: %1").arg(m_player->Level()));

    m_popLabel->setText(QString("POP: %1 / %2")
        .arg(m_player->PopulationUsed())
        .arg(m_player->PopulationCap()));

    m_roundLabel->setText(QString("Round: %1").arg(m_game->round()));
}
