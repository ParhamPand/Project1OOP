#ifndef CIRCUITESCENE_H
#define CIRCUITESCENE_H

#include <QGraphicsScene>

class CircuitScene : public QGraphicsScene
{
Q_OBJECT
public:
    explicit CircuitScene(QObject *parent = nullptr);

signals:
    void sceneClicked(const QPointF &location);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
};

#endif // CIRCUITESCENE_H
