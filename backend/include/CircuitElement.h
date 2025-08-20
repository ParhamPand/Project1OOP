#pragma once
#include "Node.h"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>

enum class ElementType {
    RESISTOR, CAPACITOR, INDUCTOR, DIODE, ZENER_DIODE,
    VOLTAGE_SOURCE, CURRENT_SOURCE, SINUSOIDAL_VOLTAGE_SOURCE, UNDEFINED
};

class CircuitElement {
    friend class Circuit;
protected:
    std::string name;
    std::shared_ptr<Node> node1;
    std::shared_ptr<Node> node2;
    double value;
    ElementType type;
    int mna_branch_index = -1;

public:
    CircuitElement() = default;
    CircuitElement(const std::string& name, std::shared_ptr<Node> n1, std::shared_ptr<Node> n2, double val, ElementType type);
    virtual ~CircuitElement();

    std::string getName() const;
    std::shared_ptr<Node> getNode1() const;
    std::shared_ptr<Node> getNode2() const;
    double getValue() const;
    ElementType getElementType() const;
    std::string getElementTypeString() const;
    virtual void setValue(double val) {
        this->value = val;
    }
    virtual void applyStamps(std::vector<std::vector<double>>& A, std::vector<double>& b, const std::map<std::string, int>& node_map, const std::vector<double>& x_prev, int mna_extra_vars_start_index, double t, double dt) const = 0;
    virtual void applyDCStamps(std::vector<std::vector<double>>& A, std::vector<double>& b, const std::map<std::string, int>& node_map, int mna_extra_vars_start_index) const = 0;
    virtual void printDetails() const;

    // تابع serialize برای کلاس پایه
    template<class Archive>
    void serialize(Archive& ar) {
        ar(CEREAL_NVP(name), CEREAL_NVP(node1), CEREAL_NVP(node2), CEREAL_NVP(value), CEREAL_NVP(type), CEREAL_NVP(mna_branch_index));
    }
};