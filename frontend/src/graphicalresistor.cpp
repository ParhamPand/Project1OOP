#include "graphicalresistor.h"
#include <QPainter>

GraphicalResistor::GraphicalResistor(QGraphicsItem *parent) : GraphicalComponent(parent){}

std::vector<QPointF> GraphicalResistor::getConnectionPoints() const
{
    return { QPointF(-45, 0), QPointF(45, 0) };
}

void GraphicalResistor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    const int bodyWidth = 60;
    const int bodyHeight = 30;
    const int leadLength = 15;

    painter->setBrush(Qt::white);
    painter->setPen(QPen(Qt::black, 2));
    QRectF bodyRect(-bodyWidth / 2, -bodyHeight / 2, bodyWidth, bodyHeight);
    painter->drawRect(bodyRect);


    painter->drawLine(-bodyWidth / 2, 0, -bodyWidth / 2 - leadLength, 0);
    painter->drawLine(bodyWidth / 2, 0, bodyWidth / 2 + leadLength, 0);

    painter->save();
    painter->rotate(-this->rotation());


    QFont font("Arial", 14, QFont::Bold);
    painter->setFont(font);
    painter->drawText(bodyRect, Qt::AlignCenter, "R");
    painter->restore();

    painter->setBrush(Qt::darkGray);
    for (const auto& point : getConnectionPoints()) {
        painter->drawRect(QRectF(point.x() - 2, point.y() - 2, 4, 4));
    }

    if (isSelected()) {

        QPen selectionPen(QColor(0, 120, 215), 2, Qt::DashLine);
        painter->setPen(selectionPen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect().adjusted(2, 2, -2, -2));
    }


}