#pragma once
#include "CircuitElement.h"
#include <memory>
#include <cereal/cereal.hpp>
#include <cereal/types/base_class.hpp>

class Resistor : public CircuitElement {
public:
    Resistor() = default; // سازنده پیش‌فرض برای Cereal
    Resistor(const std::string& name, std::shared_ptr<Node> n1, std::shared_ptr<Node> n2, double resistanceValue);
    ~Resistor() override;

    void applyStamps(std::vector<std::vector<double>>& A, std::vector<double>& b, const std::map<std::string, int>& node_map, const std::vector<double>& x_prev, int mna_extra_vars_start_index, double t, double dt) const override;
    void applyDCStamps(std::vector<std::vector<double>>& A, std::vector<double>& b, const std::map<std::string, int>& node_map, int mna_extra_vars_start_index) const override;
    void applyACStamps(std::vector<std::vector<Complex>>& A, std::vector<Complex>& b,const std::map<std::string, int>& node_map, int mna_extra_vars_start_index, double omega) const override;
    template<class Archive>
    void serialize(Archive& ar) {
        ar(cereal::base_class<CircuitElement>(this));
    }
};