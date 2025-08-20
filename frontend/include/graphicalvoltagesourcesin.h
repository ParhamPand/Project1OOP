#ifndef GRAPHICALVOLTAGESOURCESIN_H
#define GRAPHICALVOLTAGESOURCESIN_H

#include "graphicalcomponent.h"

class GraphicalVoltageSource_Sin : public GraphicalComponent
{
Q_OBJECT

public:
    explicit GraphicalVoltageSource_Sin(QGraphicsItem *parent = nullptr);

    std::vector<QPointF> getConnectionPoints() const override;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
};

#endif // GRAPHICALVOLTAGESOURCESIN_H