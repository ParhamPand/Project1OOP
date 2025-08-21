#include "graphicalcomponent.h"
#include "propertiesdialog.h"
#include "CircuitElement.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include <QKeyEvent>
#include <QGraphicsItem>
#include <QDebug>
#include <QMessageBox>
#include <regex>
#include <string>
#include <algorithm>

double parseValueWithSuffix(const std::string& raw) {
    static const std::regex pattern(R"(^([+-]?[0-9]*\.?[0-9]+(?:[eE][+-]?[0-9]+)?)([kKmMuUnN]?(?:[eE][gG])?)$)");
    std::smatch match;

    if (!std::regex_match(raw, match, pattern)) {
        throw std::invalid_argument("Invalid syntax for value.");
    }

    double baseValue = std::stod(match[1].str());
    std::string suffix = match[2].str();
    std::transform(suffix.begin(), suffix.end(), suffix.begin(), ::tolower);

    double multiplier = 1.0;
    if (suffix == "k") multiplier = 1e3;
    else if (suffix == "m") multiplier = 1e-3;
    else if (suffix == "u") multiplier = 1e-6;
    else if (suffix == "n") multiplier = 1e-9;
    else if (suffix == "meg") multiplier = 1e6;

    return baseValue * multiplier;
}

GraphicalComponent::GraphicalComponent(QGraphicsItem *parent) : QGraphicsObject(parent)
{

    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsFocusable);
}


QRectF GraphicalComponent::boundingRect() const
{

    return QRectF(-60, -60, 120, 120);
}


void GraphicalComponent::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);


    painter->setBrush(Qt::cyan);
    painter->drawEllipse(-25, -25, 50, 50);
}

void GraphicalComponent::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ClosedHandCursor);
    QGraphicsObject::mousePressEvent(event);
}

void GraphicalComponent::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mouseMoveEvent(event);
}

void GraphicalComponent::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ArrowCursor);
    QGraphicsObject::mouseReleaseEvent(event);
}

void GraphicalComponent::keyPressEvent(QKeyEvent *event)
{
    if (this->isSelected() && event->key() == Qt::Key_T)
    {
        qreal newRotation = this->rotation() + 90.0;
        this->setRotation(newRotation);
    }
    else
    {
        QGraphicsObject::keyPressEvent(event);
    }
}

void GraphicalComponent::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if (!m_logicalComponent) return;

    PropertiesDialog dialog;
    dialog.setComponentName(QString::fromStdString(m_logicalComponent->getName()));
    dialog.setCurrentValue(m_logicalComponent->getValue());

    if (dialog.exec() == QDialog::Accepted)
    {
        std::string newValueStr = dialog.getNewValueText().toStdString();

        try {
            double newValue = parseValueWithSuffix(newValueStr);


            ElementType type = m_logicalComponent->getElementType();
            if ((type == ElementType::RESISTOR || type == ElementType::CAPACITOR || type == ElementType::INDUCTOR) && newValue <= 0) {
                QMessageBox::warning(nullptr, "Invalid Value", "Value for passive components (R, C, L) must be positive.");
                return;
            }

            m_logicalComponent->setValue(newValue);
            update();

        } catch (const std::invalid_argument& e) {
            QMessageBox::warning(nullptr, "Invalid Input", "The value you entered is not valid. Please use numbers and standard suffixes (k, M, u, n).");
        }
    }
}

void GraphicalComponent::setLogicalComponent(std::shared_ptr<CircuitElement> logicalComponent)
{
    m_logicalComponent = logicalComponent;
}

std::shared_ptr<CircuitElement> GraphicalComponent::getLogicalComponent() const
{
    return m_logicalComponent;
}

QVariant GraphicalComponent::itemChange(GraphicsItemChange change, const QVariant &value)
{

    if (change == ItemPositionHasChanged) {
        emit componentMoved();
    }
    return QGraphicsObject::itemChange(change, value);
}

void GraphicalComponent::updateNodeLabels()
{

    for (auto label : m_nodeLabels) {
        delete label;
    }
    m_nodeLabels.clear();

    if (!m_logicalComponent) return;


    if (m_logicalComponent->getNode1()) {
        QString node1Name = QString::fromStdString(m_logicalComponent->getNode1()->getName());
        auto label1 = new NodeLabelItem(node1Name, this);
        QPointF pos1 = getConnectionPoints()[0];
        label1->setPos(pos1.x() - 25, pos1.y() - 25);
        m_nodeLabels.push_back(label1);
    }


    if (m_logicalComponent->getNode2() && getConnectionPoints().size() > 1) {
        QString node2Name = QString::fromStdString(m_logicalComponent->getNode2()->getName());
        auto label2 = new NodeLabelItem(node2Name, this);
        QPointF pos2 = getConnectionPoints()[1];
        label2->setPos(pos2.x() + 5, pos2.y() - 25);
        m_nodeLabels.push_back(label2);
    }
}