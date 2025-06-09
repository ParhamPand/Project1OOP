#include "SinusoidalVoltageSource.h"
#include <iostream>
#include <vector>
#include <map>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// سازنده و مخرب بدون تغییر
SinusoidalVoltageSource::SinusoidalVoltageSource(const std::string& name, Node* n1, Node* n2, double offset, double amplitude, double frequency)
        : CircuitElement(name, n1, n2, 0.0, ElementType::SINUSOIDAL_VOLTAGE_SOURCE),
          offset(offset), amplitude(amplitude), frequency(frequency) {}

SinusoidalVoltageSource::~SinusoidalVoltageSource() {}

// پیاده‌سازی تابع با امضای جدید
void SinusoidalVoltageSource::applyStamps(std::vector<std::vector<double>>& A,
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

    // KVL Stamp
    if (n1_idx != -1) A[branch_idx][n1_idx] += 1.0;
    if (n2_idx != -1) A[branch_idx][n2_idx] -= 1.0;

    // مقدار ولتاژ در سمت راست معادله، وابسته به زمان است
    double voltage_at_t = offset + amplitude * std::sin(2 * M_PI * frequency * t);
    b[branch_idx] += voltage_at_t;
}

// تابع printDetails بدون تغییر
void SinusoidalVoltageSource::printDetails() const {
    std::cout << "Element: " << getName()
              << ", Type: " << getElementTypeString()
              << ", Voffset: " << offset << "V"
              << ", Vamplitude: " << amplitude << "V"
              << ", Frequency: " << frequency << "Hz";
    if (getNode1() && getNode2()) {
        std::cout << ", Nodes: (Positive: " << getNode1()->getName()
                  << ", Negative: " << getNode2()->getName() << ")";
    }
    std::cout << std::endl;
}