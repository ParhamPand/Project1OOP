#pragma once

#include "CircuitElement.h"
#include "Node.h"
#include <string>

class Resistor : public CircuitElement {
public:
    Resistor(const std::string& name, Node* n1, Node* n2, double resistanceValue);
    ~Resistor() override;

    void applyStamps(/* CircuitMatrix& matrix */) const override;
};
