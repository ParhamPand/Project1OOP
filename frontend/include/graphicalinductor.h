#ifndef GRAPHICALINDUCTOR_H
#define GRAPHICALINDUCTOR_H

#include "graphicalcomponent.h"

class GraphicalInductor : public GraphicalComponent
{
Q_OBJECT

public:
    explicit GraphicalInductor(QGraphicsItem *parent = nullptr);

    std::vector<QPointF> getConnectionPoints() const override;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // GRAPHICALINDUCTOR_H