#ifndef GRAPHICALGROUND_H
#define GRAPHICALGROUND_H

#include "graphicalcomponent.h"

class GraphicalGround : public GraphicalComponent
{
Q_OBJECT

public:
    explicit GraphicalGround(QGraphicsItem *parent = nullptr);

    // Ground only has one connection point
    std::vector<QPointF> getConnectionPoints() const override;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // GRAPHICALGROUND_H
