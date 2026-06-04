#ifndef MAGICMIRROR_CASTVISUAL_H
#define MAGICMIRROR_CASTVISUAL_H

#include "castvisual.h"

class MagicMirrorCastVisual : public CastVisual
{
public:
    explicit MagicMirrorCastVisual(Unit* caster, int castFrames);

    void paint(QPainter* painter, const QRectF& rect) override;
};

#endif // MAGICMIRROR_CASTVISUAL_H
