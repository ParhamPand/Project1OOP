#ifndef GRAPHICALCAPACITOR_H
#define GRAPHICALCAPACITOR_H

#include "graphicalcomponent.h"

class GraphicalCapacitor : public GraphicalComponent
{
Q_OBJECT

public:
    explicit GraphicalCapacitor(QGraphicsItem *parent = nullptr);

    std::vector<QPointF> getConnectionPoints() const override;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // GRAPHICALCAPACITOR_H