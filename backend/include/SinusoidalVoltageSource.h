#pragma once
#include "CircuitElement.h"
#include <memory>
#include <cereal/cereal.hpp>
#include <cereal/types/base_class.hpp>

class SinusoidalVoltageSource : public CircuitElement {
private:
    double offset;
    double amplitude;
    double frequency;
    double phase;

public:
    // Default constructor for Cereal
    SinusoidalVoltageSource() = default;

    // Main constructor updated with shared_ptr
    SinusoidalVoltageSource(const std::string& name, std::shared_ptr<Node> n1, std::shared_ptr<Node> n2, double offset, double amplitude, double frequency, double phase);

    ~SinusoidalVoltageSource() override;

    void printDetails() const override;
    void applyStamps(std::vector<std::vector<double>>& A, std::vector<double>& b, const std::map<std::string, int>& node_map, const std::vector<double>& x_prev, int mna_extra_vars_start_index, double t, double dt) const override;
    void applyDCStamps(std::vector<std::vector<double>>& A, std::vector<double>& b, const std::map<std::string, int>& node_map, int mna_extra_vars_start_index) const override;
    void applyACStamps(std::vector<std::vector<Complex>>& A, std::vector<Complex>& b,const std::map<std::string, int>& node_map, int mna_extra_vars_start_index, double omega) const override;

    double getOffset() const { return offset; }
    double getAmplitude() const { return amplitude; }
    double getFrequency() const { return frequency; }
    double getPhase() const { return phase; }

    void setParameters(double newOffset, double newAmplitude, double newFrequency, double newPhase);


    template<class Archive>
    void serialize(Archive& ar) {
        ar(cereal::base_class<CircuitElement>(this), CEREAL_NVP(offset), CEREAL_NVP(amplitude), CEREAL_NVP(frequency), CEREAL_NVP(phase));
    }
};