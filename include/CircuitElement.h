#pragma once

#include <string>
#include "Node.h"

enum class ElementType {
    RESISTOR,
    CAPACITOR,
    INDUCTOR,
    DIODE,
    VOLTAGE_SOURCE,
    CURRENT_SOURCE,
    UNDEFINED
};

class CircuitElement {
protected:
    std::string name;
    Node* node1;
    Node* node2;
    double value;
    ElementType type;

public:
    CircuitElement(const string& name, Node* n1, Node* n2, double val, ElementType type);
    virtual ~CircuitElement();

    string getName() const;
    Node* getNode1() const;
    Node* getNode2() const;
    double getValue() const;
    ElementType getElementType() const;
    virtual string getElementTypeString() const;

    virtual void applyStamps(/* CircuitMatrix& matrix */) const = 0; // Pure virtual
    virtual void printDetails() const;
};
