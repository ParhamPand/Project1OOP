#include "VoltageSource.h"
#include <iostream>
#include <vector>
#include <map>

// سازنده و مخرب بدون تغییر
VoltageSource::VoltageSource(const std::string& name, Node* n1, Node* n2, double voltageValue)
        : CircuitElement(name, n1, n2, voltageValue, ElementType::VOLTAGE_SOURCE) {
    if (voltageValue == 0) {
        std::cout << "Info: VoltageSource '" << getName()
                  << "' created with zero value: " << voltageValue << " Volts." << std::endl;
    }
}

VoltageSource::~VoltageSource() {}

// پیاده‌سازی تابع با امضای جدید
void VoltageSource::applyStamps(std::vector<std::vector<double>>& A,
                                std::vector<double>& b,
                                const std::map<std::string, int>& node_map,
                                const std::vector<double>& x_prev,
                                int mna_extra_vars_start_index,
                                double t, double dt) const {
    int n1_idx = (node_map.count(node1->getName())) ? node_map.at(node1->getName()) : -1;
    int n2_idx = (node_map.count(node2->getName())) ? node_map.at(node2->getName()) : -1;
    int branch_idx = mna_extra_vars_start_index + this->mna_branch_index;

    // KCL Stamps
    if (n1_idx != -1) A[n1_idx][branch_idx] += 1.0;
    if (n2_idx != -1) A[n2_idx][branch_idx] -= 1.0;

    // KVL Stamp (V_n1 - V_n2 = V_source)
    if (n1_idx != -1) A[branch_idx][n1_idx] += 1.0;
    if (n2_idx != -1) A[branch_idx][n2_idx] -= 1.0;
    b[branch_idx] += getValue();
}
void VoltageSource::applyDCStamps(std::vector<std::vector<double>>& A,
                   std::vector<double>& b,
                   const std::map<std::string, int>& node_map,
                   int mna_extra_vars_start_index) const {
    int n1_idx = (node_map.count(node1->getName())) ? node_map.at(node1->getName()) : -1;
    int n2_idx = (node_map.count(node2->getName())) ? node_map.at(node2->getName()) : -1;
    int branch_idx = mna_extra_vars_start_index + this->mna_branch_index;

    // KCL Stamps
    if (n1_idx != -1) A[n1_idx][branch_idx] += 1.0;
    if (n2_idx != -1) A[n2_idx][branch_idx] -= 1.0;

    // KVL Stamp (V_n1 - V_n2 = V_source)
    if (n1_idx != -1) A[branch_idx][n1_idx] += 1.0;
    if (n2_idx != -1) A[branch_idx][n2_idx] -= 1.0;
    b[branch_idx] += getValue();
}

void VoltageSource::setValue(double val) {
    this->value = val;
}
