#pragma once

#include <string>
#include <vector>
#include <map>
#include "Node.h"

// Forward declaration to avoid circular include
class Circuit;

enum class ElementType {
    RESISTOR,
    CAPACITOR,
    INDUCTOR,
    DIODE,
    ZENER_DIODE,
    VOLTAGE_SOURCE,
    CURRENT_SOURCE,
    SINUSOIDAL_VOLTAGE_SOURCE,
    UNDEFINED
};

class CircuitElement {
    // Circuit class can access protected/private members of CircuitElement
    friend class Circuit;

protected:
    std::string name;
    Node* node1;
    Node* node2;
    double value;
    ElementType type;

    // The branch index for MNA. Only used by elements that need it.
    // Circuit class will set this directly.
    int mna_branch_index = -1;

public:
    CircuitElement(const std::string& name, Node* n1, Node* n2, double val, ElementType type);
    virtual ~CircuitElement();

    // PUBLIC INTERFACE IS CLEAN - NO SET/GET FOR BRANCH INDEX
    std::string getName() const;
    Node* getNode1() const;
    Node* getNode2() const;
    double getValue() const;
    ElementType getElementType() const;
    std::string getElementTypeString() const;

    virtual void setValue(double val) {

    }

    virtual void applyStamps(std::vector<std::vector<double>>& A,
                             std::vector<double>& b,
                             const std::map<std::string, int>& node_map,
                             const std::vector<double>& x_prev,
                             int mna_extra_vars_start_index,
                             double t, double dt) const = 0;

    virtual void applyDCStamps(std::vector<std::vector<double>>& A,
                               std::vector<double>& b,
                               const std::map<std::string, int>& node_map,
                               int mna_extra_vars_start_index) const = 0;

    virtual void printDetails() const;
};