#include "shoppanel.h"
#include "entity/player.h"
#include "core/game.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QShowEvent>
#include <QResizeEvent>

ShopPanel::ShopPanel(Player* player, Game* game, QWidget* parent)
    : QWidget(parent)
    , m_player(player)
    , m_game(game)
{
    // 覆盖层：与父窗口等大，半透明暗色背景
    setStyleSheet(R"(
        ShopPanel {
            background-color: rgba(0, 0, 0, 160);
        }
    )");
    hide();

    // 商店卡片容器
    m_card = new QWidget(this);
    m_card->setFixedSize(600, 420);
    m_card->setObjectName("shopCard");
    m_card->setStyleSheet(R"(
        #shopCard {
            background-color: #2a2a2a;
            border: 1px solid #555;
            border-radius: 8px;
        }
    )");

    auto* cardLayout = new QVBoxLayout(m_card);
    cardLayout->setContentsMargins(16, 12, 16, 16);
    cardLayout->setSpacing(10);

    // 标题栏
    auto* titleBar = new QWidget(m_card);
    auto* titleLayout = new QHBoxLayout(titleBar);
    titleLayout->setContentsMargins(0, 0, 0, 0);

    m_titleLabel = new QLabel(QStringLiteral("— 商店 —"), titleBar);
    m_titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #f1c40f;");

    m_closeButton = new QPushButton(QStringLiteral("X"), titleBar);
    m_closeButton->setFixedSize(36, 36);
    m_closeButton->setStyleSheet(R"(
        QPushButton {
            background-color: #3f3f3f;   /* 深灰底 */
            border: 1px solid #666;       /* 灰色边框 */
            border-radius: 4px;
            color: #f0f0f0;               /* 白色文字 */
            font-size: 24px;
            font-weight: bold;
            padding: 0px;
        }
        QPushButton:hover {
            background-color: #555;
            color: #fff;
        }
    )");
    connect(m_closeButton, &QPushButton::clicked, this, &QWidget::hide);

    titleLayout->addStretch();
    titleLayout->addWidget(m_titleLabel);
    titleLayout->addStretch();
    titleLayout->addWidget(m_closeButton);

    cardLayout->addWidget(titleBar);
    cardLayout->addStretch();
}

void ShopPanel::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    if (parentWidget())
        setGeometry(parentWidget()->rect());
}

void ShopPanel::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    int x = (width() - m_card->width()) / 2;
    int y = (height() - m_card->height()) / 2;
    m_card->move(x, y);
}
