#include "graphicalwire.h"
#include "graphicalcomponent.h"
#include <QPainter>

GraphicalWire::GraphicalWire(GraphicalComponent *startComp, int startTerminal, GraphicalComponent *endComp, int endTerminal, QGraphicsItem *parent)
        : QGraphicsPathItem(parent), m_startComponent(startComp), m_endComponent(endComp), m_startTerminalIndex(startTerminal), m_endTerminalIndex(endTerminal)
{
    // Get the start and end points in Scene coordinates
    QPointF startPoint = m_startComponent->mapToScene(m_startComponent->getConnectionPoints()[m_startTerminalIndex]);
    QPointF endPoint = m_endComponent->mapToScene(m_endComponent->getConnectionPoints()[m_endTerminalIndex]);

    // Create the path for the orthogonal wire
    QPainterPath path;
    path.moveTo(startPoint);

    // Simple orthogonal routing logic
    QPointF cornerPoint(endPoint.x(), startPoint.y());
    path.lineTo(cornerPoint);
    path.lineTo(endPoint);

    // Set the path for this item to draw
    setPath(path);
    setPen(QPen(Qt::black, 2));
}

void GraphicalWire::updatePosition()
{
    // This is the same logic from the constructor, just re-run
    QPointF startPoint = m_startComponent->mapToScene(m_startComponent->getConnectionPoints()[m_startTerminalIndex].position);
    QPointF endPoint = m_endComponent->mapToScene(m_endComponent->getConnectionPoints()[m_endTerminalIndex].position);

    QPainterPath path;
    path.moveTo(startPoint);

    // Use the smart routing logic
    if (m_startComponent->getConnectionPoints()[m_startTerminalIndex].orientation == TerminalOrientation::Horizontal) {
        QPointF cornerPoint(endPoint.x(), startPoint.y());
        path.lineTo(cornerPoint);
    } else {
        QPointF cornerPoint(startPoint.x(), endPoint.y());
        path.lineTo(cornerPoint);
    }
    path.lineTo(endPoint);

    setPath(path); // Update the wire's path
}