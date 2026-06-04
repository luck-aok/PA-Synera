#include "mermaid_castvisual.h"
#include "entity/unit.h"
#include <QPainter>

MermaidCastVisual::MermaidCastVisual(Unit* caster, int castFrames)
    : CastVisual(caster, castFrames)
{}

void MermaidCastVisual::paint(QPainter* painter, const QRectF& rect)
{
    if (!m_caster || m_caster->isDead()) return;

    float p = progress();

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    // 海蓝泡沫，从淡→亮，忽大忽小
    int alpha = static_cast<int>(60 + 120 * p);
    qreal scale = 0.7f + 0.3f * p;

    QColor sea(80, 200, 220, alpha);   /* 海蓝色 */
    QPen pen(sea, 2.0 + 2.0 * p);
    painter->setPen(pen);
    painter->setBrush(QColor(80, 200, 220, alpha / 2));

    qreal cx = rect.center().x();
    qreal cy = rect.center().y();
    qreal radius = qMax(rect.width(), rect.height()) * 0.5 * scale;

    painter->translate(cx, cy);
    painter->rotate(p * 180);
    painter->translate(-cx, -cy);

    painter->drawEllipse(QPointF(cx, cy), radius, radius);
    painter->restore();
}
