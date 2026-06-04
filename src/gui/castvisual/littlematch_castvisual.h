#ifndef LITTLEMATCH_CASTVISUAL_H
#define LITTLEMATCH_CASTVISUAL_H

#include "castvisual.h"

class LittleMatchCastVisual : public CastVisual
{
public:
    explicit LittleMatchCastVisual(Unit* caster, int castFrames);

    void paint(QPainter* painter, const QRectF& rect) override;
};

#endif // LITTLEMATCH_CASTVISUAL_H
