#ifndef GRAPHICALCURRENTSOURCEDC_H
#define GRAPHICALCURRENTSOURCEDC_H

#include "graphicalcomponent.h"

class GraphicalCurrentSource_DC : public GraphicalComponent
{
Q_OBJECT

public:
    explicit GraphicalCurrentSource_DC(QGraphicsItem *parent = nullptr);

    std::vector<QPointF> getConnectionPoints() const override;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // GRAPHICALCURRENTSOURCEDC_H
