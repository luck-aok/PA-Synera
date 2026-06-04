#ifndef SNOWWHITE_CASTVISUAL_H
#define SNOWWHITE_CASTVISUAL_H

#include "castvisual.h"

class SnowWhiteCastVisual : public CastVisual
{
public:
    explicit SnowWhiteCastVisual(Unit* caster, int castFrames);

    void paint(QPainter* painter, const QRectF& rect) override;
};

#endif // SNOWWHITE_CASTVISUAL_H
