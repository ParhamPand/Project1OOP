#pragma once

#include "CircuitElement.h"

class SinusoidalVoltageSource : public CircuitElement {
private:
    double offset;
    double amplitude;
    double frequency;

public:
    SinusoidalVoltageSource(const std::string& name, Node* n1, Node* n2, double offset, double amplitude, double frequency);
    ~SinusoidalVoltageSource() override;

    // این تابع را override می‌کنیم تا تمام جزئیات را نمایش دهد
    void printDetails() const override;
    void applyStamps(std::vector<std::vector<double>>& A,
                     std::vector<double>& b,
                     const std::map<std::string, int>& node_map,
                     const std::vector<double>& x_prev,
                     int mna_extra_vars_start_index,
                     double t, double dt) const override;

    // Getters for specific parameters
    double getOffset() const { return offset; }
    double getAmplitude() const { return amplitude; }
    double getFrequency() const { return frequency; }
};
