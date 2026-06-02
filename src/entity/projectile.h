#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <QGraphicsItem>
#include <QPointF>

class Unit;
class BoardGeometry;

class Projectile
{
public:
    Projectile(Unit* target, int damage, BoardGeometry* geo, int duration = 20);
    virtual ~Projectile();

    // 每帧调一次。返回 true = 已到达目标，该清理了
    virtual bool tick();

    QGraphicsItem* item() const { return m_item; }
    bool arrived() const { return m_arrived; }
    int damage() const { return m_damage; }
    Unit* target() const { return m_target; }

    // 设置发射起点（像素坐标），显示图形
    void setStartPos(const QPointF& pixelPos);

protected:
    Unit* m_target;         // 目标单位（每帧跟踪其位置）
    int m_damage;           // 到达后造成的伤害值
    BoardGeometry* m_geo;   // 坐标转换工具（逻辑坐标→像素坐标）
    int m_duration;         // 最大飞行帧数（超时自动消失）
    int m_elapsed = 0;      // 已经飞了多少帧
    QGraphicsItem* m_item = nullptr;  // 场景中的显示图形
    QPointF m_pos;          // 当前像素位置
    bool m_arrived = false; // true = 已到达目标或已超时
};

// 默认子弹：橙色小球，每帧追着目标飞
class SimpleProjectile : public Projectile
{
public:
    SimpleProjectile(Unit* target, int damage, BoardGeometry* geo);
    bool tick() override;
};

#endif // PROJECTILE_H
