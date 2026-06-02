#ifndef BOARDGEOMETRY_H
#define BOARDGEOMETRY_H

#include <QPoint>
#include <QPointF>
#include <QPolygonF>

class BoardGeometry
{
public:
    BoardGeometry(int rows, int cols, qreal m_cellSize);

    int rows() const { return m_rows; }
    int cols() const { return m_cols; }
    qreal cellSize() const {return m_cellSize;}

    QPointF gridToWorld(int row, int col) const;
    QPoint worldToGrid(const QPointF& world) const;

    int benchSlotAt(const QPointF& scenePos, int benchSlots) const;
    QPointF benchSlotToWorld(int slot, int benchSlots) const;

private:
    int m_rows;
    int m_cols;
    qreal m_cellSize;
};

#endif // BOARDGEOMETRY_H
