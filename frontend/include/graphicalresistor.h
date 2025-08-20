#ifndef GRAPHICALRESISTOR_H
#define GRAPHICALRESISTOR_H

#include "graphicalcomponent.h"

class GraphicalResistor : public GraphicalComponent
{
Q_OBJECT

public:
    explicit GraphicalResistor(QGraphicsItem *parent = nullptr);
    std::vector<QPointF> getConnectionPoints() const override;


protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // GRAPHICALRESISTOR_H
