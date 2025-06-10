#pragma once

#include <string>
#include <vector>
#include <map>
#include <stdexcept> // For std::runtime_error
#include <set>

// Forward declarations
class Node;
class CircuitElement;
class Resistor;
class Capacitor;
class Inductor;
class VoltageSource;
class SinusoidalVoltageSource;
class CurrentSource;
class Diode;
class ZenerDiode;

class Circuit {
private:
    std::set<std::string> groundedNodes;
    std::map<std::string, Node*> namedNodes;
    std::vector<CircuitElement*> allElements;
    int nextNodeId;
    int mna_extra_vars_count;

    Node* getOrCreateNode(const std::string& nodeName);


    CircuitElement* getElement(const std::string& name) const;

    void performVoltageTransientAnalysis(double t_step, double t_stop, const std::string& output_node_name);
    void performCurrentTransientAnalysis(double t_step, double t_stop, const std::string& output_element_name);


public:
    Circuit();
    ~Circuit();

    Node* addNode(const std::string& name);

    Resistor* addResistor(const std::string& name, const std::string& node1Name, const std::string& node2Name, double resistance);
    Capacitor* addCapacitor(const std::string& name, const std::string& node1Name, const std::string& node2Name, double capacitance);
    Inductor* addInductor(const std::string& name, const std::string& node1Name, const std::string& node2Name, double inductance);
    VoltageSource* addVoltageSource(const std::string& name, const std::string& positiveNodeName, const std::string& negativeNodeName, double voltage);
    SinusoidalVoltageSource* addSinusoidalVoltageSource(const std::string& name, const std::string& node1Name, const std::string& node2Name, double offset, double amplitude, double frequency);
    CurrentSource* addCurrentSource(const std::string& name, const std::string& fromNodeName, const std::string& toNodeName, double current);
    Diode* addDiode(const std::string& name, const std::string& anodeName, const std::string& cathodeName);
    ZenerDiode* addZenerDiode(const std::string& name, const std::string& anodeName, const std::string& cathodeName);


    Node* getNode(const std::string& name) const;
    const std::vector<CircuitElement*>& getElements() const;
    const std::map<std::string, Node*>& getNodes() const;
    void handleCommand(const std::string& input);

    void printCircuitDetails() const;
    void printNodes() const;
    void listElements() const;
    void listElementsByType(const std::string& typeName) const;

    void renameNode(const std::string& oldName, const std::string& newName);


    Circuit(const Circuit&) = delete;
    Circuit& operator=(const Circuit&) = delete;
};
