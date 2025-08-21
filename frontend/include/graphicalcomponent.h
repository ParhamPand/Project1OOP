#ifndef GRAPHICALCOMPONENT_H
#define GRAPHICALCOMPONENT_H

#include "nodelabelitem.h"
#include <QGraphicsObject>
#include <QKeyEvent>
#include <QPointF>
#include <memory>
#include <vector>


class QGraphicsSceneContextMenuEvent;
class CircuitElement;


class GraphicalComponent : public QGraphicsObject
{
Q_OBJECT

public:
    explicit GraphicalComponent(QGraphicsItem *parent = nullptr);
    void setLogicalComponent(std::shared_ptr<CircuitElement> logicalComponent);
    std::shared_ptr<CircuitElement> getLogicalComponent() const;
    void updateNodeLabels();


    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual std::vector<QPointF> getConnectionPoints() const = 0;

    signals:
    void componentMoved();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    std::shared_ptr<CircuitElement> m_logicalComponent;
    std::vector<NodeLabelItem*> m_nodeLabels;


};

#endif // GRAPHICALCOMPONENT_H