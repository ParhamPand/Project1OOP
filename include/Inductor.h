#pragma once

#include "CircuitElement.h"
#include "Node.h"
#include <string>

class Inductor : public CircuitElement {
public:
    Inductor(const std::string& name, Node* n1, Node* n2, double inductanceValue);
    ~Inductor() override;

    void applyStamps(/* CircuitMatrix& matrix */) const override;
};
