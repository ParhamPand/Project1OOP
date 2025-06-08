#pragma once

#include "CircuitElement.h"
#include "Node.h"
#include <string>

class ZenerDiode : public CircuitElement {
public:
    ZenerDiode(const std::string& name, Node* n1, Node* n2);
    ~ZenerDiode() override;

    void applyStamps(/* CircuitMatrix& matrix */) const override;
};
