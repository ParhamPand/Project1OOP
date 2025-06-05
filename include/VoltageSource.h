#pragma once

#include "CircuitElement.h"
#include "Node.h"
#include <string>

class VoltageSource : public CircuitElement {
public:
    // n1 is positive terminal, n2 is negative terminal
    VoltageSource(const std::string& name, Node* n1, Node* n2, double voltageValue);
    ~VoltageSource() override;

    void applyStamps(/* CircuitMatrix& matrix */) const override;
    // double getVoltage() const { return getValue(); } // Alias for getValue
};

