#ifndef GRAPHICALWIRE_H
#define GRAPHICALWIRE_H

#include <QGraphicsPathItem>

class GraphicalComponent; // Forward declaration

class GraphicalWire : public QGraphicsPathItem
{
public:
    GraphicalWire(GraphicalComponent *startComp, int startTerminal, GraphicalComponent *endComp, int endTerminal, QGraphicsItem *parent = nullptr);

    // Add any other necessary functions here

public slots:
    void updatePosition();


private:
    GraphicalComponent *m_startComponent;
    GraphicalComponent *m_endComponent;
    int m_startTerminalIndex;
    int m_endTerminalIndex;
};

#endif // GRAPHICALWIRE_H