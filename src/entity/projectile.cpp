#include "projectile.h"
#include "entity/unit.h"
#include "core/boardgeometry.h"
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QColor>
#include <QtMath>

Projectile::Projectile(Unit* target, int damage, BoardGeometry* geo, int duration)
    : m_target(target), m_damage(damage), m_geo(geo), m_duration(duration)
{}

Projectile::~Projectile()
{
    delete m_item;
}

void Projectile::setStartPos(const QPointF& pixelPos)
{
    m_pos = pixelPos;
    m_item->setPos(pixelPos);
    m_item->setVisible(true);
}

bool Projectile::tick()
{
    return m_elapsed >= m_duration;
}

// ---- SimpleProjectile ----

SimpleProjectile::SimpleProjectile(Unit* target, int damage, BoardGeometry* geo)
    : Projectile(target, damage, geo, 20)
{
    auto* ellipse = new QGraphicsEllipseItem(-4, -4, 8, 8);
    ellipse->setBrush(QColor(255, 200, 50));
    ellipse->setPen(Qt::NoPen);
    ellipse->setZValue(3.0);
    ellipse->setVisible(false);
    m_item = ellipse;
}

bool SimpleProjectile::tick()
{
    if (m_arrived) return true;

    if (!m_target || m_target->isDead()) {
        m_arrived = true;
        return true;
    }

    m_elapsed++;

    // 每帧重新拿目标当前位置，直线飞过去
    QPoint tgtPos = m_target->position();
    QPointF targetPixel = m_geo->gridToWorld(tgtPos.y(), tgtPos.x());

    qreal dx = targetPixel.x() - m_pos.x();
    qreal dy = targetPixel.y() - m_pos.y();
    qreal dist = qSqrt(dx * dx + dy * dy);

    qreal speed = 6.0;

    if (dist < speed + 2.0) {
        m_arrived = true;
        m_pos = targetPixel;
    } else {
        m_pos.setX(m_pos.x() + dx / dist * speed);
        m_pos.setY(m_pos.y() + dy / dist * speed);
    }

    m_item->setPos(m_pos);
    return m_arrived;
}
