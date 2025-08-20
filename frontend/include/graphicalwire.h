#ifndef GRAPHICALWIRE_H
#define GRAPHICALWIRE_H

// به جای QGraphicsPathItem از QGraphicsObject استفاده کنید
#include <QGraphicsObject>
#include <QPainterPath>
#include <QPen>

class GraphicalComponent; // Forward declaration

// ارث‌بری را از QGraphicsPathItem به QGraphicsObject تغییر دهید
class GraphicalWire : public QGraphicsObject
{
Q_OBJECT // <-- ماکروی Q_OBJECT را اضافه کنید

public:
    GraphicalWire(GraphicalComponent *startComp, int startTerminal, GraphicalComponent *endComp, int endTerminal, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

public slots:
    void updatePosition();

private:
    GraphicalComponent *m_startComponent;
    GraphicalComponent *m_endComponent;
    int m_startTerminalIndex;
    int m_endTerminalIndex;

    QPainterPath m_path;
    QPen m_pen;
};

#endif