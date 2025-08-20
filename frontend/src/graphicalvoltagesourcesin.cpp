#include "graphicalvoltagesourcesin.h"
#include "sinusoidalsourcedialog.h"
#include "SinusoidalVoltageSource.h"
#include <QPainter>
#include <QPainterPath>

GraphicalVoltageSource_Sin::GraphicalVoltageSource_Sin(QGraphicsItem *parent) : GraphicalComponent(parent) {}

std::vector<QPointF> GraphicalVoltageSource_Sin::getConnectionPoints() const
{
    // Terminals at the top (+) and bottom (-)
    return { QPointF(0, -30), QPointF(0, 30) };
}

void GraphicalVoltageSource_Sin::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    const int radius = 20;
    const int leadLength = 10;

    painter->setPen(QPen(Qt::black, 2));
    painter->setBrush(Qt::white);

    // Draw the main circle body
    painter->drawEllipse(-radius, -radius, 2 * radius, 2 * radius);

    // Draw the leads
    painter->drawLine(0, -radius, 0, -radius - leadLength); // Top lead
    painter->drawLine(0, radius, 0, radius + leadLength);   // Bottom lead

    // Draw the sine wave symbol inside
    QPainterPath sinePath;
    sinePath.moveTo(-radius * 0.7, 0);
    sinePath.quadTo(-radius * 0.35, -radius * 0.7, 0, 0);
    sinePath.quadTo(radius * 0.35, radius * 0.7, radius * 0.7, 0);
    painter->drawPath(sinePath);

    // Draw connection points
    painter->setBrush(Qt::darkGray);
    for (const auto& point : getConnectionPoints()) {
        painter->drawRect(QRectF(point.x() - 2, point.y() - 2, 4, 4));
    }

    // Draw selection indicator
    if (isSelected()) {
        QPen selectionPen(QColor(0, 120, 215), 2, Qt::DashLine);
        painter->setPen(selectionPen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect());
    }
}

void GraphicalVoltageSource_Sin::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{

    if (!m_logicalComponent) return;

    auto sinSource = std::dynamic_pointer_cast<SinusoidalVoltageSource>(m_logicalComponent);
    if (!sinSource) return;

    SinusoidalSourceDialog dialog;


    dialog.setOffset(sinSource->getOffset());
    dialog.setAmplitude(sinSource->getAmplitude());
    dialog.setFrequency(sinSource->getFrequency());
    dialog.setPhase(sinSource->getPhase());


    if (dialog.exec() == QDialog::Accepted)
    {

        double newOffset = dialog.getOffset();
        double newAmp = dialog.getAmplitude();
        double newFreq = dialog.getFrequency();
        double newPhase = dialog.getPhase();

        sinSource->setParameters(newOffset, newAmp, newFreq, newPhase);

        update();
    }
}