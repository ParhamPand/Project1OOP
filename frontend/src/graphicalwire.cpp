#include "graphicalwire.h"
#include "graphicalcomponent.h"
#include <QPainter>

GraphicalWire::GraphicalWire(GraphicalComponent *startComp, int startTerminal, GraphicalComponent *endComp, int endTerminal, QGraphicsItem *parent)
        : QGraphicsObject(parent), m_startComponent(startComp), m_endComponent(endComp), m_startTerminalIndex(startTerminal), m_endTerminalIndex(endTerminal)
{
    // به جای setPen(...)، متغیر عضو m_pen را مقداردهی کنید
    m_pen = QPen(Qt::black, 2);

    setZValue(-1);
    updatePosition();
}

QRectF GraphicalWire::boundingRect() const
{
    return m_path.boundingRect().adjusted(-2, -2, 2, 2);
}

void GraphicalWire::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);


    painter->setPen(m_pen);
    painter->drawPath(m_path);
}


void GraphicalWire::updatePosition()
{
    prepareGeometryChange();

    QPointF startPoint = m_startComponent->mapToScene(m_startComponent->getConnectionPoints()[m_startTerminalIndex]);
    QPointF endPoint = m_endComponent->mapToScene(m_endComponent->getConnectionPoints()[m_endTerminalIndex]);

    m_path = QPainterPath();
    m_path.moveTo(mapFromScene(startPoint));
    QPointF cornerPoint(mapFromScene(endPoint).x(), mapFromScene(startPoint).y());
    m_path.lineTo(cornerPoint);
    m_path.lineTo(mapFromScene(endPoint));

    update();
}