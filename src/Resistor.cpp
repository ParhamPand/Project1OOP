#include "Resistor.h"
#include <iostream>
#include <vector>
#include <map>

// سازنده و مخرب بدون تغییر
Resistor::Resistor(const std::string& name, Node* n1, Node* n2, double resistanceValue)
        : CircuitElement(name, n1, n2, resistanceValue, ElementType::RESISTOR) {
    if (resistanceValue <= 0) {
        std::cout << "Warning: Resistor '" << getName()
                  << "' created with non-positive value: " << resistanceValue << " Ohms." << std::endl;
    }
}

Resistor::~Resistor() {}

// پیاده‌سازی تابع با امضای جدید
void Resistor::applyStamps(std::vector<std::vector<double>>& A,
                           std::vector<double>& b,
                           const std::map<std::string, int>& node_map,
                           const std::vector<double>& x_prev,
                           int mna_extra_vars_start_index,
                           double t, double dt) const {
    double g = 1.0 / value;

    int n1_idx = (node_map.count(node1->getName())) ? node_map.at(node1->getName()) : -1;
    int n2_idx = (node_map.count(node2->getName())) ? node_map.at(node2->getName()) : -1;

    // یک تابع کمکی برای جلوگیری از دسترسی به ایندکس‌های نامعتبر (مثل گره زمین)
    auto add_to_A = [&](int r, int c, double val) {
        if (r >= 0 && c >= 0) {
            A[r][c] += val;
        }
    };

    add_to_A(n1_idx, n1_idx, g);
    add_to_A(n2_idx, n2_idx, g);
    add_to_A(n1_idx, n2_idx, -g);
    add_to_A(n2_idx, n1_idx, -g);
}