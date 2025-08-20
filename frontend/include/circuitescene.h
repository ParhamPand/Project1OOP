#ifndef CIRCUITESCENE_H
#define CIRCUITESCENE_H

#include <QGraphicsScene>

class GraphicalComponent;

class CircuitScene : public QGraphicsScene
{
Q_OBJECT
public:
    explicit CircuitScene(QObject *parent = nullptr);

signals:
    void connectionMade(GraphicalComponent *startComp, int startTerminalIndex, GraphicalComponent *endComp, int endTerminalIndex);
    void sceneClicked(const QPointF &location);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

private:
    bool m_isWiring = false;
    QPointF m_startPoint;
    QGraphicsLineItem *m_previewLine = nullptr;

    GraphicalComponent *m_startComponent = nullptr;
    int m_startTerminalIndex = -1;
};

#endif // CIRCUITESCENE_H
