#include "crystalshoes_castvisual.h"
#include "entity/unit.h"
#include <QPainter>
#include <QtMath>

CrystalShoesCastVisual::CrystalShoesCastVisual(Unit* caster, int castFrames)
    : CastVisual(caster, castFrames)
{}

void CrystalShoesCastVisual::paint(QPainter* painter, const QRectF& rect)
{
    if (!m_caster || m_caster->isDead()) return;

    float p = progress();

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    int alpha = static_cast<int>(100 + 100 * p);
    qreal penWidth = 1.5 + 3.0 * p;

    QColor gold(255, 200, 50, alpha);
    QPen pen(gold, penWidth);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);

    qreal cx = rect.center().x();
    qreal cy = rect.center().y();
    qreal radius = qMax(rect.width(), rect.height()) * 0.6;

    painter->translate(cx, cy);
    painter->rotate(p * 360);
    painter->translate(-cx, -cy);

    painter->drawEllipse(QPointF(cx, cy), radius, radius);
    painter->restore();
}
