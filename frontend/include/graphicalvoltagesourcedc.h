#ifndef GRAPHICALVOLTAGESOURCEDC_H
#define GRAPHICALVOLTAGESOURCEDC_H

#include "graphicalcomponent.h"

class GraphicalVoltageSource_DC : public GraphicalComponent
{
Q_OBJECT

public:
    explicit GraphicalVoltageSource_DC(QGraphicsItem *parent = nullptr);

    std::vector<QPointF> getConnectionPoints() const override;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // GRAPHICALVOLTAGESOURCEDC_H