#include "silvermane_castvisual.h"
#include "entity/unit.h"
#include <QPainter>

SilvermaneCastVisual::SilvermaneCastVisual(Unit* caster, int castFrames)
    : CastVisual(caster, castFrames)
{}

void SilvermaneCastVisual::paint(QPainter* painter, const QRectF& rect)
{
    if (!m_caster || m_caster->isDead()) return;

    float p = progress();

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    // 血色光环，从淡→浓
    int alpha = static_cast<int>(60 + 140 * p);
    qreal penWidth = 2.0 + 3.0 * p;

    QColor blood(200, 40, 60, alpha);   /* 暗红色 */
    QPen pen(blood, penWidth);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);

    qreal cx = rect.center().x();
    qreal cy = rect.center().y();
    qreal radius = qMax(rect.width(), rect.height()) * 0.55;

    // 反向旋转
    painter->translate(cx, cy);
    painter->rotate(-p * 360);
    painter->translate(-cx, -cy);

    painter->drawEllipse(QPointF(cx, cy), radius, radius);
    painter->restore();
}
