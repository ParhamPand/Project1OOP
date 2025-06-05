#pragma once

#include "CircuitElement.h"
#include "Node.h"
#include <string>

class CurrentSource : public CircuitElement {
public:
    // Current flows from n1 towards n2 through the external circuit
    CurrentSource(const std::string& name, Node* n1, Node* n2, double currentValue);
    ~CurrentSource() override;

    void applyStamps(/* CircuitMatrix& matrix */) const override;
    // double getCurrent() const { return getValue(); } // Alias for getValue
};

