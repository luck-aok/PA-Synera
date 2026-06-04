#include "snowwhite_castvisual.h"
#include "entity/unit.h"
#include <QPainter>

SnowWhiteCastVisual::SnowWhiteCastVisual(Unit* caster, int castFrames)
    : CastVisual(caster, castFrames)
{}

void SnowWhiteCastVisual::paint(QPainter* painter, const QRectF& rect)
{
    if (!m_caster || m_caster->isDead()) return;

    float p = progress();

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    // 水晶玻璃光泽，从淡蓝→亮白
    int alpha = static_cast<int>(80 + 120 * p);
    qreal radius = qMax(rect.width(), rect.height()) * 0.5 * (0.7f + 0.3f * p);

    QColor glass(180, 220, 255, alpha);   /* 淡蓝玻璃色 */
    QPen pen(glass, 2.5);
    painter->setPen(pen);
    painter->setBrush(QColor(180, 220, 255, alpha / 3));   /* 半透明填充 */

    qreal cx = rect.center().x();
    qreal cy = rect.center().y();

    painter->drawEllipse(QPointF(cx, cy), radius, radius);

    // 内层十字闪光
    painter->setPen(QPen(QColor(255, 255, 255, alpha), 1.0));
    painter->drawLine(QPointF(cx - radius * 0.6, cy), QPointF(cx + radius * 0.6, cy));
    painter->drawLine(QPointF(cx, cy - radius * 0.6), QPointF(cx, cy + radius * 0.6));

    painter->restore();
}
