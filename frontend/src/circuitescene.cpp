#include "circuitescene.h"
#include <QGraphicsSceneMouseEvent>

CircuitScene::CircuitScene(QObject *parent) : QGraphicsScene(parent) {}

void CircuitScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::LeftButton) {
        emit sceneClicked(mouseEvent->scenePos());
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}
