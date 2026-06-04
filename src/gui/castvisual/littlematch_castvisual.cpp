#include "littlematch_castvisual.h"
#include "entity/unit.h"
#include <QPainter>

LittleMatchCastVisual::LittleMatchCastVisual(Unit* caster, int castFrames)
    : CastVisual(caster, castFrames)
{}

void LittleMatchCastVisual::paint(QPainter* painter, const QRectF& rect)
{
    if (!m_caster || m_caster->isDead()) return;

    float p = progress();

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    // 火焰光环：橙→红，从淡→亮，跳跃闪烁
    int alpha = static_cast<int>(80 + 140 * p);
    qreal penWidth = 1.5 + 3.5 * p;

    QColor fire(255, 140, 30, alpha);   /* 橙红色火焰 */
    QPen pen(fire, penWidth);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);

    qreal cx = rect.center().x();
    qreal cy = rect.center().y();
    qreal baseRadius = qMax(rect.width(), rect.height()) * 0.5;
    qreal radius = baseRadius * (0.6f + 0.4f * p);

    painter->translate(cx, cy);
    painter->rotate(p * 360);
    painter->translate(-cx, -cy);

    painter->drawEllipse(QPointF(cx, cy), radius, radius);

    // 内层第二圈（火焰跳跃感）
    QColor fireInner(255, 200, 60, alpha / 2);   /* 亮橙色内焰 */
    painter->setPen(QPen(fireInner, penWidth * 0.5));
    painter->drawEllipse(QPointF(cx, cy), radius * 0.7, radius * 0.7);

    painter->restore();
}
