#ifndef MERMAID_CASTVISUAL_H
#define MERMAID_CASTVISUAL_H

#include "castvisual.h"

class MermaidCastVisual : public CastVisual
{
public:
    explicit MermaidCastVisual(Unit* caster, int castFrames);

    void paint(QPainter* painter, const QRectF& rect) override;
};

#endif // MERMAID_CASTVISUAL_H
