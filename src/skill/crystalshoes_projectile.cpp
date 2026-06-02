#include "crystalshoes_projectile.h"
#include "entity/unit.h"
#include "core/boardgeometry.h"
#include <QGraphicsPixmapItem>
#include <QCoreApplication>
#include <QFileInfo>
#include <QtMath>

CrystalShoesProjectile::CrystalShoesProjectile(Unit* target, int damage, BoardGeometry* geo)
    : Projectile(target, damage, geo, 40)
{
    auto* pixItem = new QGraphicsPixmapItem();
    pixItem->setZValue(3.0);
    pixItem->setVisible(false);

    const QString appDir = QCoreApplication::applicationDirPath();
    const QString roots[] = {
        QFileInfo(appDir + "/..").canonicalFilePath(),
        QFileInfo(appDir + "/../..").canonicalFilePath()
    };
    for (const QString& root : roots) {
        if (root.isEmpty()) continue;
        QPixmap pix(root + "/assets/cinderella/crystalshoe.png");
        if (!pix.isNull()) {
            pixItem->setPixmap(pix.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            pixItem->setOffset(-24, -24);
            break;
        }
    }

    m_item = pixItem;
}

void CrystalShoesProjectile::setStartPos(const QPointF& pixelPos)
{
    m_pos = pixelPos;
    m_item->setPos(pixelPos);
    m_item->setVisible(true);
}

bool CrystalShoesProjectile::tick()
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

    // 每帧旋转 10 度
    qreal angle = m_elapsed * 10;
    m_item->setRotation(angle);

    return m_arrived;
}
