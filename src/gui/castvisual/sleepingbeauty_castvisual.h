#ifndef SLEEPINGBEAUTY_CASTVISUAL_H
#define SLEEPINGBEAUTY_CASTVISUAL_H

#include "castvisual.h"

class SleepingBeautyCastVisual : public CastVisual
{
public:
    explicit SleepingBeautyCastVisual(Unit* caster, int castFrames);

    void paint(QPainter* painter, const QRectF& rect) override;
};

#endif // SLEEPINGBEAUTY_CASTVISUAL_H
