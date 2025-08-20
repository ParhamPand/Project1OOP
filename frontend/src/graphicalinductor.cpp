#include "graphicalinductor.h"
#include <QPainter>
#include <QFont>
#include <QPainterPath>

GraphicalInductor::GraphicalInductor(QGraphicsItem *parent) : GraphicalComponent(parent) {}

std::vector<QPointF> GraphicalInductor::getConnectionPoints() const
{
    return { QPointF(-45, 0), QPointF(45, 0) };
}

void GraphicalInductor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    const int leadLength = 15;
    const int coilRadius = 10;
    const int totalCoilWidth = 3 * (2 * coilRadius); // عرض کلی سه سیم‌پیچ

    painter->setPen(QPen(Qt::black, 2));
    painter->setBrush(Qt::NoBrush);

    // پایه‌ها
    painter->drawLine(-45, 0, -totalCoilWidth/2, 0); // پایه چپ
    painter->drawLine(totalCoilWidth/2, 0, 45, 0);   // پایه راست

    // سیم‌پیچ (مجموعه‌ای از نیم‌دایره‌ها)
    QPainterPath path;
    path.arcMoveTo(-totalCoilWidth/2, -coilRadius, 2 * coilRadius, 2 * coilRadius, 180);
    path.arcTo(-totalCoilWidth/2, -coilRadius, 2 * coilRadius, 2 * coilRadius, 180, -180);
    path.arcTo(-totalCoilWidth/2 + (2*coilRadius), -coilRadius, 2 * coilRadius, 2 * coilRadius, 180, -180);
    path.arcTo(-totalCoilWidth/2 + (4*coilRadius), -coilRadius, 2 * coilRadius, 2 * coilRadius, 180, -180);
    painter->drawPath(path);

    // نقاط اتصال
    painter->setBrush(Qt::darkGray);
    for (const auto& point : getConnectionPoints()) {
        painter->drawRect(QRectF(point.x() - 2, point.y() - 2, 4, 4));
    }

    // نقاشی متن "L" بدون چرخش
    painter->save();
    painter->rotate(-this->rotation());
    QFont font("Arial", 14, QFont::Bold);
    painter->setFont(font);
    painter->drawText(boundingRect(), Qt::AlignCenter, "L");
    painter->restore();

    if (isSelected()) {
        QPen selectionPen(QColor(0, 120, 215), 2, Qt::DashLine);
        painter->setPen(selectionPen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect().adjusted(2, 2, -2, -2));
    }
}
