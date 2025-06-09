#pragma once

#include "CircuitElement.h"

class Resistor : public CircuitElement {
public:
    Resistor(const std::string& name, Node* n1, Node* n2, double resistanceValue);
    ~Resistor() override;


    void applyStamps(std::vector<std::vector<double>>& A,
                     std::vector<double>& b,
                     const std::map<std::string, int>& node_map,
                     const std::vector<double>& x_prev,
                     int mna_extra_vars_start_index,
                     double t, double dt) const override;
};