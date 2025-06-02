#include "CircuitElement.h"
#include <iostream>

using namespace std;

CircuitElement::CircuitElement(const string& name, Node* n1, Node* n2, double val, ElementType type)
        : name(name), node1(n1), node2(n2), value(val), type(type) {
}

CircuitElement::~CircuitElement() {
    // Element does not own nodes, so no deletion here
}

string CircuitElement::getName() const {
    return name;
}

Node* CircuitElement::getNode1() const {
    return node1;
}

Node* CircuitElement::getNode2() const {
    return node2;
}

double CircuitElement::getValue() const {
    return value;
}

ElementType CircuitElement::getElementType() const {
    return type;
}

string CircuitElement::getElementTypeString() const {
    switch (type) {
        case ElementType::RESISTOR: return "Resistor";
        case ElementType::CAPACITOR: return "Capacitor";
        case ElementType::INDUCTOR: return "Inductor";
        case ElementType::DIODE: return "Diode";
        case ElementType::VOLTAGE_SOURCE: return "VoltageSource";
        case ElementType::CURRENT_SOURCE: return "CurrentSource";
        default: return "Undefined";
    }
}

void CircuitElement::printDetails() const {
    cout << "Element: " << getName()
              << ", Type: " << getElementTypeString()
              << ", Value: " << getValue();
    if (node1 && node2) {
        cout << ", Nodes: (" << node1->getName() << " [ID:" << node1->getId() << "]"
                  << ", " << node2->getName() << " [ID:" << node2->getId() << "])";
    } else {
        cout << ", Nodes: (One or both nodes are null)";
    }
    cout << endl;
}
