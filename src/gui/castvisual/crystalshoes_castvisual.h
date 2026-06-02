#ifndef CRYSTALSHOES_CASTVISUAL_H
#define CRYSTALSHOES_CASTVISUAL_H

#include "castvisual.h"

class CrystalShoesCastVisual : public CastVisual
{
public:
    explicit CrystalShoesCastVisual(Unit* caster, int castFrames);

    void paint(QPainter* painter, const QRectF& rect) override;
};

#endif // CRYSTALSHOES_CASTVISUAL_H
