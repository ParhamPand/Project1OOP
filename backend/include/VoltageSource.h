#pragma once

#include "CircuitElement.h"

class VoltageSource : public CircuitElement {
public:
    VoltageSource(const std::string& name, Node* n1, Node* n2, double voltageValue);
    ~VoltageSource() override;

    void setValue(double val) override;

    void applyStamps(std::vector<std::vector<double>>& A,
                     std::vector<double>& b,
                     const std::map<std::string, int>& node_map,
                     const std::vector<double>& x_prev,
                     int mna_extra_vars_start_index,
                     double t, double dt) const override;

    void applyDCStamps(std::vector<std::vector<double>>& A,
                       std::vector<double>& b,
                       const std::map<std::string, int>& node_map,
                       int mna_extra_vars_start_index) const override;
};