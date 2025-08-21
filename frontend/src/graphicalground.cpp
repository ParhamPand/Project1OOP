#include "graphicalground.h"
#include <QPainter>

GraphicalGround::GraphicalGround(QGraphicsItem *parent) : GraphicalComponent(parent) {}


std::vector<QPointF> GraphicalGround::getConnectionPoints() const
{
    return { QPointF(0, -15) };
}

void GraphicalGround::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(QPen(Qt::black, 2));
    painter->setBrush(Qt::NoBrush);

    // The vertical lead from the top
    painter->drawLine(0, -15, 0, 0);

    // The three shrinking horizontal lines
    painter->drawLine(-20, 0, 20, 0);
    painter->drawLine(-15, 5, 15, 5);
    painter->drawLine(-10, 10, 10, 10);

    // Draw the single connection point
    painter->setBrush(Qt::darkGray);
    for (const auto& point : getConnectionPoints()) {
        painter->drawRect(QRectF(point.x() - 2, point.y() - 2, 4, 4));
    }

    // Draw the selection indicator
    if (isSelected()) {
        QPen selectionPen(QColor(0, 120, 215), 2, Qt::DashLine);
        painter->setPen(selectionPen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect());
    }
}