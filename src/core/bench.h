#ifndef BENCH_H
#define BENCH_H

#include <QHash>
#include <QPoint>
#include <QVector>
#include "entity/unit.h"

class Bench{
public:
    static constexpr int Slots= 8;

    Bench();
    ~Bench()=default;

public:
    void addUnit(Unit* unit,int slot);
    void removeUnit(Unit* unit);//从格子移除
    Unit* getUnitAt(int slot) const;//查某格子上是谁
    bool isSlotEmpty(int slot)const;
    bool isfull()const;
    int slotOf(Unit* unit)const; //已知单位 → 查它在哪个槽位
    void clear();
private:
    QVector<Unit*> m_slots;// 8 个格子，存指针（nullptr 表示空格）
    QHash<Unit*, int> m_unitToSlot; // 反向索引：从 Unit 查坐标
};
#endif // BENCH_H
