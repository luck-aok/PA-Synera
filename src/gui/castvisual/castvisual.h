#ifndef CASTVISUAL_H
#define CASTVISUAL_H

class QPainter;
class QRectF;
class Unit;

class CastVisual
{
public:
    explicit CastVisual(Unit* caster, int castFrames);//构造函数只有 1个必须参数时加 explicit 基本不会错,后面变成两个了，但也不改了，无害，原来是担心Unit被隐式转换
    virtual ~CastVisual() = default;

    void tick();
    float progress() const;
    void reset();

    virtual void paint(QPainter* painter, const QRectF& rect) = 0;

    Unit* caster() const { return m_caster; }

protected:
    Unit* m_caster;
    int m_totalFrames;
    int m_elapsed = 0;
};

#endif // CASTVISUAL_H
