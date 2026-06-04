#include "sleepingbeauty_castvisual.h"
#include "entity/unit.h"
#include <QPainter>

SleepingBeautyCastVisual::SleepingBeautyCastVisual(Unit* caster, int castFrames)
    : CastVisual(caster, castFrames)
{}

void SleepingBeautyCastVisual::paint(QPainter* painter, const QRectF& rect)
{
    if (!m_caster || m_caster->isDead()) return;

    float p = progress();

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    // 紫色荆棘漩涡，从淡→亮
    int alpha = static_cast<int>(70 + 130 * p);
    qreal penWidth = 1.5 + 3.0 * p;

    QColor thorn(160, 80, 200, alpha);   /* 紫色荆棘 */
    QPen pen(thorn, penWidth);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);

    qreal cx = rect.center().x();
    qreal cy = rect.center().y();
    qreal radius = qMax(rect.width(), rect.height()) * 0.5;

    painter->translate(cx, cy);
    painter->rotate(p * 360);
    painter->translate(-cx, -cy);

    painter->drawEllipse(QPointF(cx, cy), radius, radius);
    painter->restore();
}
