#include "graphicalvoltagesourcedc.h"
#include <QPainter>
#include <QFont>

GraphicalVoltageSource_DC::GraphicalVoltageSource_DC(QGraphicsItem *parent) : GraphicalComponent(parent) {}

std::vector<QPointF> GraphicalVoltageSource_DC::getConnectionPoints() const
{
    // Terminals at the top (+) and bottom (-)
    return { QPointF(0, -30), QPointF(0, 30) };
}

void GraphicalVoltageSource_DC::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    const int radius = 20;
    const int leadLength = 10;

    painter->setPen(QPen(Qt::black, 2));
    painter->setBrush(Qt::white);

    // Draw the main circle body
    painter->drawEllipse(-radius, -radius, 2 * radius, 2 * radius);

    // Draw the leads
    painter->drawLine(0, -radius, 0, -radius - leadLength); // Top lead (+)
    painter->drawLine(0, radius, 0, radius + leadLength);   // Bottom lead (-)

    // Draw the polarity signs
    QFont font("Arial", 16, QFont::Bold);
    painter->setFont(font);
    painter->drawText(QRectF(-radius, -radius, 2 * radius, radius), Qt::AlignCenter | Qt::AlignBottom, "+");
    painter->drawText(QRectF(-radius, 0, 2 * radius, radius), Qt::AlignCenter | Qt::AlignTop, "-");

    // Draw connection points
    painter->setBrush(Qt::darkGray);
    for (const auto& point : getConnectionPoints()) {
        painter->drawRect(QRectF(point.x() - 2, point.y() - 2, 4, 4));
    }

    // Draw selection indicator
    if (isSelected()) {
        QPen selectionPen(QColor(0, 120, 215), 2, Qt::DashLine);
        painter->setPen(selectionPen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect());
    }
}