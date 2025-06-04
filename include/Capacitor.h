#pragma once

#include "CircuitElement.h"
#include "Node.h"
#include <string>

class Capacitor : public CircuitElement {
public:
    Capacitor(const std::string& name, Node* n1, Node* n2, double capacitanceValue);
    ~Capacitor() override;

    void applyStamps(/* CircuitMatrix& matrix */) const override;
};
