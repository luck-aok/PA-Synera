#ifndef UNINVITED_CASTVISUAL_H
#define UNINVITED_CASTVISUAL_H

#include "castvisual.h"

class UninvitedCastVisual : public CastVisual
{
public:
    explicit UninvitedCastVisual(Unit* caster, int castFrames);

    void paint(QPainter* painter, const QRectF& rect) override;
};

#endif // UNINVITED_CASTVISUAL_H
