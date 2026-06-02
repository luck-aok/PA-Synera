#include "boardgeometry.h"
#include <QtMath>

BoardGeometry::BoardGeometry(int rows, int cols, qreal cellSize)
    : m_rows(rows), m_cols(cols), m_cellSize(cellSize)
{}

QPointF BoardGeometry::gridToWorld(int row, int col) const
{
    qreal x = col * m_cellSize + m_cellSize / 2.0;
    qreal y = row * m_cellSize + m_cellSize / 2.0;
    return QPointF(x, y);
}

QPoint BoardGeometry::worldToGrid(const QPointF& world) const
{
    int col = static_cast<int>(world.x() / m_cellSize);
    int row = static_cast<int>(world.y() / m_cellSize);

    col = qBound(0, col, m_cols - 1);
    row = qBound(0, row, m_rows - 1);

    return QPoint(col, row);
}

int BoardGeometry::benchSlotAt(const QPointF& scenePos, int benchSlots) const
{
    qreal boardBottom = m_rows*m_cellSize;
    qreal benchY = boardBottom + 50.0;

    if (scenePos.y() < benchY || scenePos.y() > benchY + 80.0) {
        return -1;
    }

    qreal totalWidth = benchSlots * 70.0;
    qreal startX = (m_cols*m_cellSize) / 2.0 - totalWidth / 2.0;

    int slot = static_cast<int>((scenePos.x() - startX) / 70.0);
    if (slot < 0 || slot >= benchSlots) return -1;
    return slot;
}

QPointF BoardGeometry::benchSlotToWorld(int slot, int benchSlots) const
{
    qreal boardBottom = m_rows * m_cellSize;
    qreal benchY = boardBottom + 50.0 + 40.0;

    qreal totalWidth = benchSlots * 70.0;
    qreal startX = (m_cols * m_cellSize) / 2.0 - totalWidth / 2.0;

    return QPointF(startX + slot * 70.0 + 35.0, benchY);
}
