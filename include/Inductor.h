#pragma once

#include "CircuitElement.h"

class Inductor : public CircuitElement {
public:
    Inductor(const std::string& name, Node* n1, Node* n2, double inductanceValue);
    ~Inductor() override;

    // امضای تابع باید با کلاس پدر مطابقت داشته باشد
    void applyStamps(std::vector<std::vector<double>>& A,
                     std::vector<double>& b,
                     const std::map<std::string, int>& node_map,
                     const std::vector<double>& x_prev,
                     int mna_extra_vars_start_index,
                     double t, double dt) const override;
};