#ifndef BOARD_H
#define BOARD_H

#include <QHash>
#include <QPoint>
#include <QVector>
#include "entity/unit.h"

class Board
{
public:
    static constexpr int ROWS = 8;
    static constexpr int COLS = 8;

    Board();
    ~Board() = default;

    void addUnit(Unit* unit, const QPoint& pos);//在格子放单位（有检查：unit 为空/坐标非法/格子被占 → return）
    void removeUnit(Unit* unit);//从格子移除
    Unit* getUnitAt(const QPoint& pos) const;//查某格子上是谁
    bool hasUnitAt(const QPoint& pos) const;//某格子是否被占

    bool isValidPosition(const QPoint& pos) const;//坐标在 0~7 范围内
    bool isPlayerHalf(const QPoint& pos) const;//y >= 4（下半场）

    void clear();

private:
    int indexOf(const QPoint& pos) const;

    QVector<Unit*> m_cells;// 8×8=64 个格子，存指针（nullptr 表示空格）
    QHash<Unit*, QPoint> m_unitToPosition; // 反向索引：从 Unit 查坐标
};

#endif // BOARD_H
