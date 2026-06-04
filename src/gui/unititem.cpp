#include "gui/unititem.h"
#include "entity/unit.h"
#include "gui/castvisual/castvisual.h"
#include <QCoreApplication>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QFileInfo>

UnitItem::UnitItem(Unit* unit, QGraphicsItem* parent)
    : QGraphicsObject(parent)
    , m_unit(unit)
    , m_gridPos(-1, -1)
    , m_dragging(false)
    , m_spriteTried(false)
{
    setAcceptedMouseButtons(Qt::LeftButton);
}

QRectF UnitItem::boundingRect() const
{
    return QRectF(-42, -42, 84, 104);
}

void UnitItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
//画角色（没有就画小牌牌），阴影
    painter->setRenderHint(QPainter::Antialiasing);
    ensureSpriteLoaded();

    if (!m_sprite.isNull()) {
        const QRectF targetRect(-40, -40, 80, 80);
        painter->drawPixmap(targetRect, m_sprite, m_sprite.rect());
    }else{
        painter->setPen(Qt::NoPen);
        //这里补充区分敌我逻辑
        QColor shadowColor = (m_unit && m_unit->owner()==Unit::Owner::EnemyCtrl)?
                                 QColor(180,40,40,110):QColor(20,20,20,110);//红色半透明：默认灰黑
        painter->setBrush(shadowColor);
        painter->drawEllipse(QRectF(-14, 8, 28, 10));

        QPolygonF badge;
        badge << QPointF(0, -15)
              << QPointF(13, -7)
              << QPointF(13, 7)
              << QPointF(0, 15)
              << QPointF(-13, 7)
              << QPointF(-13, -7);

        painter->setPen(QPen(QColor(18, 18, 18), 1.5));
        painter->setBrush(QColor(100, 150, 200));
        painter->drawPolygon(badge);

        if (m_unit) {
            painter->setPen(Qt::white);
            QFont font = painter->font();
            font.setPointSize(12);
            font.setBold(true);
            painter->setFont(font);
            painter->drawText(QRectF(-13, -13, 26, 26), Qt::AlignCenter, m_unit->name().left(1));
        }
    }

//这部分在画血条和蓝条
    if(!m_unit)return;
    const qreal barW = 50.0;
    const qreal barH = 5.0;

    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(40,40,40));
    painter->drawRoundedRect(QRectF(-25,32,barW,barH),1,1);

    qreal hpRatio = qreal(m_unit->hp())/qreal(m_unit->maxHp());
    if(hpRatio < 0)hpRatio=0.0;
    if(hpRatio > 1)hpRatio=1.0;

    QColor hpColor = (hpRatio >0.3)?QColor(80,200,80):QColor(220,60,60);
    painter->setBrush(hpColor);
    painter->drawRoundedRect(QRectF(-25,32,barW*hpRatio,barH),1,1);

    painter->setPen(QPen(QColor(90,90,90), 1));
    painter->setBrush(QColor(45,45,45));
    painter->drawRoundedRect(QRectF(-25,39,barW,4),1,1);

    qreal manaRatio = qreal(m_unit->mana())/qreal(m_unit->maxMana());
    if(manaRatio < 0)manaRatio=0.0;
    if(manaRatio > 1)manaRatio=1.0;

    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(60,120,220));
    painter->drawRoundedRect(QRectF(-25,39,barW*manaRatio,4),1,1);

//施法蓄力特效
    if (m_unit->state() == Unit::State::Casting) {
        CastVisual* cv = m_unit->castVisual();
        if (cv)
            cv->paint(painter, QRectF(-30, -30, 60, 60));
    }

//这部分画敌我标识：底部小原点
    QColor ownerColor =
        (m_unit->owner() == Unit::Owner::PlayerCtrl) ?
                            QColor(60,180,255) : QColor(220,50,50);//蓝：红
    painter->setPen(Qt::NoPen);
    painter->setBrush(ownerColor);
    painter->drawEllipse((QRect(-4,44,9,9)));

}

void UnitItem::ensureSpriteLoaded() const
{
    if (m_spriteTried) {
        return;
    }

    m_spriteTried = true;
    const QString relativePath = spriteRelativePathForUnit();
    if (relativePath.isEmpty()) {
        return;
    }

    const QString appDir = QCoreApplication::applicationDirPath();
    const QString roots[] = {
        QFileInfo(appDir + "/..").canonicalFilePath(),
        QFileInfo(appDir + "/../..").canonicalFilePath()
    };

    QPixmap pix;
    for (const QString& root : roots) {
        if (root.isEmpty()) {
            continue;
        }
        pix.load(root + "/" + relativePath);
        if (!pix.isNull()) {
            break;
        }
    }

    if (pix.isNull()) {
        return;
    }

    m_sprite = pix.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

QString UnitItem::spriteRelativePathForUnit() const
{
    if (!m_unit) {
        return QString();
    }

    const QString name = m_unit->name();
    if (name == QString::fromUtf8("战术")) {
        return QStringLiteral("assets/craftpix-reaper-man-chibi-2d-game-sprites/Reaper_Man_1/PNG/PNG Sequences/Idle/0_Reaper_Man_Idle_000.png");
    }
    if (name == QString::fromUtf8("弓手")) {
        return QStringLiteral("assets/craftpix-satyr-tiny-style-2d-sprites/PNG/Satyr_01/PNG Sequences/Idle/Satyr_01_Idle_000.png");
    }
    if (name == QString::fromUtf8("法师")) {
        return QStringLiteral("assets/craftpix-wraith-tiny-style-2d-sprites/PNG/Wraith_02/PNG Sequences/Idle/Wraith_02_Idle_000.png");
    }
    if (name == QString::fromUtf8("灰姑娘")) {
        return QStringLiteral("assets/cinderella/7dfcca7d-7c9c-4564-8bf3-fa5bb997bf56.png");
    }
    if (name == QString::fromUtf8("大灰狼")){
        return QStringLiteral("assets/silvrmane/1e8f9073-1be0-42cf-b88a-0f8cd218901b.png");
    }
    if (name == QString::fromUtf8("魔镜")){
        return QStringLiteral("assets/magicmirror/magicmirror_2.png");
    }
    if (name == QString::fromUtf8("人鱼")){
        return QStringLiteral("assets/mermaid/mermaid_2.png");
    }
    if (name == QString::fromUtf8("白雪")){
        return QStringLiteral("assets/snowwhite/snowwhite_1.png");
    }
    if (name == QString::fromUtf8("小火苗")){
        return QStringLiteral("assets/littlematch/littlematch_1.png");
    }
    if (name == QString::fromUtf8("无邀者")){
        return QStringLiteral("assets/Uninvited/Uninvited_1.png");
    }
    if (name == QString::fromUtf8("睡美人")){
        return QStringLiteral("assets/sleepbeauty/sleepbeauty_2.png");
    }
    return QString();
}

int UnitItem::unitId() const
{
    return m_unit ? m_unit->id() : -1;
}

void UnitItem::setGridPos(const QPoint& gridPos)
{
    m_gridPos = gridPos;
}

void UnitItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() != Qt::LeftButton) {
        QGraphicsObject::mousePressEvent(event);
        return;
    }

    m_dragging = true;
    emit dragStarted(unitId(), m_gridPos, event->scenePos());
    event->accept();
}

void UnitItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (!m_dragging) {
        QGraphicsObject::mouseMoveEvent(event);
        return;
    }

    emit dragMoved(unitId(), m_gridPos, event->scenePos());
    event->accept();
}

void UnitItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (!m_dragging || event->button() != Qt::LeftButton) {
        QGraphicsObject::mouseReleaseEvent(event);
        return;
    }

    m_dragging = false;
    emit dragDropped(unitId(), m_gridPos, event->scenePos());
    event->accept();
}
