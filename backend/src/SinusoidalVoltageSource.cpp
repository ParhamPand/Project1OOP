#include "SinusoidalVoltageSource.h"
#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <complex>


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Constructor signature updated to accept shared_ptr
SinusoidalVoltageSource::SinusoidalVoltageSource(const std::string& name, std::shared_ptr<Node> n1, std::shared_ptr<Node> n2, double offset, double amplitude, double frequency, double phase)
        : CircuitElement(name, n1, n2, 0.0, ElementType::SINUSOIDAL_VOLTAGE_SOURCE),
          offset(offset), amplitude(amplitude), frequency(frequency) , phase(phase) {}

SinusoidalVoltageSource::~SinusoidalVoltageSource() {}

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

    // The voltage value on the right-hand side is time-dependent
    double phase_rad = phase * M_PI / 180.0;
    double voltage_at_t = offset + amplitude * std::sin(frequency * t + phase_rad);
    b[branch_idx] += voltage_at_t;
}

void SinusoidalVoltageSource::applyDCStamps(std::vector<std::vector<double>>& A,
                                            std::vector<double>& b,
                                            const std::map<std::string, int>& node_map,
                                            int mna_extra_vars_start_index) const {
    // In DC analysis, a sinusoidal source might be treated as a DC source with its offset value,
    // or as a short circuit (0V). Here we'll treat it as its offset value.
    int n1_idx = (node_map.count(node1->getName())) ? node_map.at(node1->getName()) : -1;
    int n2_idx = (node_map.count(node2->getName())) ? node_map.at(node2->getName()) : -1;
    int branch_idx = mna_extra_vars_start_index + this->mna_branch_index;

    // KCL Stamps
    if (n1_idx != -1) A[n1_idx][branch_idx] += 1.0;
    if (n2_idx != -1) A[n2_idx][branch_idx] -= 1.0;

    // KVL Stamp
    if (n1_idx != -1) A[branch_idx][n1_idx] += 1.0;
    if (n2_idx != -1) A[branch_idx][n2_idx] -= 1.0;
    b[branch_idx] += this->offset; // Use the DC offset for DC analysis
}

void SinusoidalVoltageSource::applyACStamps(std::vector<std::vector<Complex>>& A, std::vector<Complex>& b, const std::map<std::string, int>& node_map, int mna_extra_vars_start_index, double omega) const {
    int n1_idx = (node_map.count(node1->getName())) ? node_map.at(node1->getName()) : -1;
    int n2_idx = (node_map.count(node2->getName())) ? node_map.at(node2->getName()) : -1;
    int branch_idx = mna_extra_vars_start_index + this->mna_branch_index;

    if (n1_idx != -1) A[n1_idx][branch_idx] += 1.0;
    if (n2_idx != -1) A[n2_idx][branch_idx] -= 1.0;


    if (n1_idx != -1) A[branch_idx][n1_idx] += 1.0;
    if (n2_idx != -1) A[branch_idx][n2_idx] -= 1.0;

    double phase_rad = this->phase * M_PI / 180.0;

    Complex phasor_value = std::polar(this->amplitude, phase_rad);

    b[branch_idx] += phasor_value;
}

void SinusoidalVoltageSource::setParameters(double newOffset, double newAmplitude, double newFrequency, double newPhase)
{
    this->offset = newOffset;
    this->amplitude = newAmplitude;
    this->frequency = newFrequency;
    this->phase = newPhase;
}

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