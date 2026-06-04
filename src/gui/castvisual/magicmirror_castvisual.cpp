#include "magicmirror_castvisual.h"
#include "entity/unit.h"
#include <QPainter>

MagicMirrorCastVisual::MagicMirrorCastVisual(Unit* caster, int castFrames)
    : CastVisual(caster, castFrames)
{}

void MagicMirrorCastVisual::paint(QPainter* painter, const QRectF& rect)
{
    if (!m_caster || m_caster->isDead()) return;

    float p = progress();

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    // 银色镜光，从淡→亮
    int alpha = static_cast<int>(60 + 140 * p);
    qreal scale = 0.6f + 0.4f * p;   /* 光环从缩到放 */

    QColor silver(180, 190, 210, alpha);   /* 银灰色 */
    QPen pen(silver, 2.0 + 2.0 * p);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);

    qreal cx = rect.center().x();
    qreal cy = rect.center().y();
    qreal baseRadius = qMax(rect.width(), rect.height()) * 0.5;
    qreal radius = baseRadius * scale;

    painter->translate(cx, cy);
    painter->rotate(p * 180);    /* 慢速旋转 */
    painter->translate(-cx, -cy);

    painter->drawEllipse(QPointF(cx, cy), radius, radius);
    painter->restore();
}
