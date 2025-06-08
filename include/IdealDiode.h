#pragma once

#include "CircuitElement.h"
#include "Node.h"
#include <string>

class Diode : public CircuitElement {
private:
    string model;

public:
    Diode(const std::string& name, Node* n1, Node* n2, const std::string& model);
    ~Diode() override;

    std::string getModel() const; // تابع برای دسترسی به مدل
    void applyStamps(/* CircuitMatrix& matrix */) const override;
};
