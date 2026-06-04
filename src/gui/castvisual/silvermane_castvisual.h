#ifndef SILVERMANE_CASTVISUAL_H
#define SILVERMANE_CASTVISUAL_H

#include "gui/castvisual/castvisual.h"

class SilvermaneCastVisual : public CastVisual
{
public:
    explicit SilvermaneCastVisual(Unit* caster, int castFrames);

    void paint(QPainter* painter, const QRectF& rect) override;
};

#endif // SILVERMANE_CASTVISUAL_H
