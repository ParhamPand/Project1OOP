#ifndef GRAPHICALCOMPONENT_H
#define GRAPHICALCOMPONENT_H

#include <QGraphicsObject>
#include <QKeyEvent>
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


    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual std::vector<QPointF> getConnectionPoints() const = 0;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    std::shared_ptr<CircuitElement> m_logicalComponent;


};

#endif // GRAPHICALCOMPONENT_H