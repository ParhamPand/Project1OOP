#pragma once

#include "CircuitElement.h"
#include "Node.h"
#include <string>

class Diode : public CircuitElement {
public:
    Diode(const std::string& name, Node* n1, Node* n2);
    ~Diode() override;

    void applyStamps(/* CircuitMatrix& matrix */) const override;
};
