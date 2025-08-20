#include "graphicalcapacitor.h"
#include <QPainter>
#include <QFont>

GraphicalCapacitor::GraphicalCapacitor(QGraphicsItem *parent) : GraphicalComponent(parent) {}

std::vector<QPointF> GraphicalCapacitor::getConnectionPoints() const
{

    return { QPointF(-45, 0), QPointF(45, 0) };
}

void GraphicalCapacitor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    const int plateGap = 20;
    const int plateLength = 60;
    const int leadLength = 45;

    painter->setPen(QPen(Qt::black, 2));
    painter->setBrush(Qt::NoBrush);

    painter->drawLine(-leadLength, 0, -plateGap / 2, 0); // پایه چپ
    painter->drawLine(-plateGap / 2, -plateLength / 2, -plateGap / 2, plateLength / 2); // صفحه چپ
    painter->drawLine(plateGap / 2, -plateLength / 2, plateGap / 2, plateLength / 2);  // صفحه راست
    painter->drawLine(plateGap / 2, 0, leadLength, 0);   // پایه راست

    painter->setBrush(Qt::darkGray);
    for (const auto& point : getConnectionPoints()) {
        painter->drawRect(QRectF(point.x() - 2, point.y() - 2, 4, 4));
    }

    painter->save();
    painter->rotate(-this->rotation());
    QFont font("Arial", 14, QFont::Bold);
    painter->setFont(font);
    painter->drawText(boundingRect(), Qt::AlignCenter, "C");
    painter->restore();

    if (isSelected()) {
        QPen selectionPen(QColor(0, 120, 215), 2, Qt::DashLine);
        painter->setPen(selectionPen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect().adjusted(2, 2, -2, -2));
    }
}