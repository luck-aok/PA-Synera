#include "uninvited_castvisual.h"
#include "entity/unit.h"
#include <QPainter>

UninvitedCastVisual::UninvitedCastVisual(Unit* caster, int castFrames)
    : CastVisual(caster, castFrames)
{}

void UninvitedCastVisual::paint(QPainter* painter, const QRectF& rect)
{
    if (!m_caster || m_caster->isDead()) return;

    float p = progress();

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    // 暗紫色诅咒漩涡，从淡→浓
    int alpha = static_cast<int>(50 + 150 * p);
    qreal penWidth = 1.0 + 3.0 * p;

    QColor darkPurple(100, 40, 140, alpha);   /* 暗紫色诅咒 */
    QPen pen(darkPurple, penWidth);
    pen.setStyle(Qt::DashLine);
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
