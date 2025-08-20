#include "Capacitor.h"
#include <iostream>
#include <vector>
#include <map>

// امضای سازنده با shared_ptr آپدیت شده
Capacitor::Capacitor(const std::string& name, std::shared_ptr<Node> n1, std::shared_ptr<Node> n2, double capacitanceValue)
        : CircuitElement(name, n1, n2, capacitanceValue, ElementType::CAPACITOR) {
    if (capacitanceValue <= 0) {
        std::cout << "Warning: Capacitor '" << getName()
                  << "' created with non-positive value: " << capacitanceValue << " Farads." << std::endl;
    }
}

Capacitor::~Capacitor() {}

// بقیه توابع کلاس بدون تغییر باقی می‌مانند
void Capacitor::applyStamps(std::vector<std::vector<double>>& A,
                            std::vector<double>& b,
                            const std::map<std::string, int>& node_map,
                            const std::vector<double>& x_prev,
                            int mna_extra_vars_start_index,
                            double t, double dt) const {
    if (dt <= 1e-12) return;

    double C = getValue();
    double g_eq = C / dt;

    int n1_idx = (node_map.count(node1->getName())) ? node_map.at(node1->getName()) : -1;
    int n2_idx = (node_map.count(node2->getName())) ? node_map.at(node2->getName()) : -1;

    auto add_to_A = [&](int r, int c, double val) {
        if (r >= 0 && c >= 0) A[r][c] += val;
    };
    auto add_to_b = [&](int r, double val) {
        if (r >= 0) b[r] += val;
    };

    add_to_A(n1_idx, n1_idx, g_eq);
    add_to_A(n2_idx, n2_idx, g_eq);
    add_to_A(n1_idx, n2_idx, -g_eq);
    add_to_A(n2_idx, n1_idx, -g_eq);

    double v_prev1 = (n1_idx != -1 && n1_idx < x_prev.size()) ? x_prev[n1_idx] : 0.0;
    double v_prev2 = (n2_idx != -1 && n2_idx < x_prev.size()) ? x_prev[n2_idx] : 0.0;
    double v_c_prev = v_prev1 - v_prev2;

    double i_eq = g_eq * v_c_prev;
    add_to_b(n1_idx, i_eq);
    add_to_b(n2_idx, -i_eq);
}

void Capacitor::applyDCStamps(std::vector<std::vector<double>>& A,
                              std::vector<double>& b,
                              const std::map<std::string, int>& node_map,
                              int mna_extra_vars_start_index) const {
}