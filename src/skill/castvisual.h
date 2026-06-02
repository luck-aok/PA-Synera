#ifndef CASTVISUAL_H
#define CASTVISUAL_H

class QPainter;
class QRectF;

class CastVisual
{
public:
    virtual ~CastVisual() = default;
    virtual void paint(QPainter* painter, const QRectF& rect, float progress) = 0;
};

#endif // CASTVISUAL_H
