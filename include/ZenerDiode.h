#pragma once

#include "CircuitElement.h"
#include "Node.h"
#include <string>
#include <vector>
#include <map>

class ZenerDiode : public CircuitElement {
public:
    ZenerDiode(const std::string& name, Node* n1, Node* n2);
    ~ZenerDiode() override;

    // فقط این خط تغییر کرده است
    void applyStamps(std::vector<std::vector<double>>& A,
                     std::vector<double>& b,
                     const std::map<std::string, int>& node_map,
                     const std::vector<double>& x_prev,
                     int mna_extra_vars_start_index,
                     double t, double dt) const override;
};