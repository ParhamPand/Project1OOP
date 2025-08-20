#include "circuitescene.h"
#include "graphicalcomponent.h"
#include "mainwindow.h"
#include "graphicalwire.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>

CircuitScene::CircuitScene(QObject *parent) : QGraphicsScene(parent) {}

void CircuitScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    // Check if we are already in the middle of drawing a wire
    if (m_isWiring) {
        // --- This is the logic for the SECOND click ---

        // Find the component and terminal under the second click
        QList<QGraphicsItem*> itemsUnderMouse = items(mouseEvent->scenePos());
        for (auto *item : itemsUnderMouse) {
            auto endComponent = dynamic_cast<GraphicalComponent*>(item);
            if (endComponent && endComponent != m_startComponent) { // Ensure it's a component and not the starting one
                auto connectionPoints = endComponent->getConnectionPoints();
                for (size_t i = 0; i < connectionPoints.size(); ++i) {
                    QPointF scenePoint = endComponent->mapToScene(connectionPoints[i]);
                    if (QLineF(mouseEvent->scenePos(), scenePoint).length() < 10.0) {
                        // --- SUCCESSFUL CONNECTION ---
                        // 1. Create the permanent wire
                        auto permanentWire = new GraphicalWire(m_startComponent, m_startTerminalIndex, endComponent, i);
                        addItem(permanentWire);

                        connect(m_startComponent, &GraphicalComponent::componentMoved, permanentWire, &GraphicalWire::updatePosition);
                        connect(endComponent, &GraphicalComponent::componentMoved, permanentWire, &GraphicalWire::updatePosition);

                        // 2. Emit the signal to tell MainWindow to update the backend
                        emit connectionMade(m_startComponent, m_startTerminalIndex, endComponent, i);

                        // 3. Clean up and exit wiring mode
                        removeItem(m_previewLine);
                        delete m_previewLine;
                        m_previewLine = nullptr;
                        m_isWiring = false;
                        m_startComponent = nullptr;
                        return; // Done
                    }
                }
            }
        }
        // If the user clicked in an empty space, cancel the wiring
        removeItem(m_previewLine);
        delete m_previewLine;
        m_previewLine = nullptr;
        m_isWiring = false;
        m_startComponent = nullptr;
        return;

    } else if (views().at(0)->property("currentMode").toInt() == static_cast<int>(PlacementMode::Wire)) {
        // --- This is the logic for the FIRST click (largely the same as before) ---
        QList<QGraphicsItem*> itemsUnderMouse = items(mouseEvent->scenePos());
        for (auto *item : itemsUnderMouse) {
            auto component = dynamic_cast<GraphicalComponent*>(item);
            if (component) {
                auto connectionPoints = component->getConnectionPoints();
                for (size_t i = 0; i < connectionPoints.size(); ++i) {
                    QPointF scenePoint = component->mapToScene(connectionPoints[i]);
                    if (QLineF(mouseEvent->scenePos(), scenePoint).length() < 10.0) {
                        m_isWiring = true;
                        m_startPoint = scenePoint;
                        m_startComponent = component; // Store the starting component
                        m_startTerminalIndex = i;     // Store the starting terminal index

                        m_previewLine = new QGraphicsLineItem(QLineF(m_startPoint, m_startPoint));
                        m_previewLine->setPen(QPen(Qt::gray, 2, Qt::DashLine));
                        addItem(m_previewLine);
                        return;
                    }
                }
            }
        }
    } else {
        if (mouseEvent->button() == Qt::LeftButton) {
            emit sceneClicked(mouseEvent->scenePos());
        }
    }

    QGraphicsScene::mousePressEvent(mouseEvent);
}


void CircuitScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_isWiring && m_previewLine) {
        m_previewLine->setLine(QLineF(m_startPoint, mouseEvent->scenePos()));
    }
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}