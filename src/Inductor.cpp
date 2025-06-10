#include "Inductor.h"
#include <iostream>
#include <vector>
#include <map>

// سازنده و مخرب بدون تغییر
Inductor::Inductor(const std::string& name, Node* n1, Node* n2, double inductanceValue)
        : CircuitElement(name, n1, n2, inductanceValue, ElementType::INDUCTOR) {
    if (inductanceValue <= 0) {
        std::cout << "Warning: Inductor '" << getName()
                  << "' created with non-positive value: " << inductanceValue << " Henrys." << std::endl;
    }
}

Inductor::~Inductor() {}

// پیاده‌سازی تابع با امضای جدید
void Inductor::applyStamps(std::vector<std::vector<double>>& A,
                           std::vector<double>& b,
                           const std::map<std::string, int>& node_map,
                           const std::vector<double>& x_prev,
                           int mna_extra_vars_start_index,
                           double t, double dt) const {
    if (dt <= 1e-12) return;

    double L = getValue();
    int n1_idx = (node_map.count(node1->getName())) ? node_map.at(node1->getName()) : -1;
    int n2_idx = (node_map.count(node2->getName())) ? node_map.at(node2->getName()) : -1;
    int branch_idx = mna_extra_vars_start_index + this->mna_branch_index;

    // KCL part (جریان سلف بر KCL گره‌ها تأثیر می‌گذارد)
    if (n1_idx != -1) A[n1_idx][branch_idx] += 1.0;
    if (n2_idx != -1) A[n2_idx][branch_idx] -= 1.0;

    // KVL part (معادله جدید برای جریان سلف)
    // V1 - V2 - (L/dt)*I_L(t) = -(L/dt)*I_L(t-1)
    if (n1_idx != -1) A[branch_idx][n1_idx] += 1.0;
    if (n2_idx != -1) A[branch_idx][n2_idx] -= 1.0;
    A[branch_idx][branch_idx] -= L / dt;

    double i_L_prev = (branch_idx < x_prev.size()) ? x_prev[branch_idx] : 0.0;
    b[branch_idx] += (L / dt) * i_L_prev;
}