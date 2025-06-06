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
class VoltageSource; // <-- اضافه شده
class CurrentSource; // <-- اضافه شده

class Circuit {
private:
    std::set<std::string> groundedNodes;
    std::map<std::string, Node*> namedNodes;
    std::vector<CircuitElement*> allElements;
    int nextNodeId;

    Node* getOrCreateNode(const std::string& nodeName);

public:
    Circuit();
    ~Circuit();

    Node* addNode(const std::string& name);

    Resistor* addResistor(const std::string& name, const std::string& node1Name, const std::string& node2Name, double resistance);
    Capacitor* addCapacitor(const std::string& name, const std::string& node1Name, const std::string& node2Name, double capacitance);
    Inductor* addInductor(const std::string& name, const std::string& node1Name, const std::string& node2Name, double inductance);
    VoltageSource* addVoltageSource(const std::string& name, const std::string& positiveNodeName, const std::string& negativeNodeName, double voltage); // <-- اضافه شده
    CurrentSource* addCurrentSource(const std::string& name, const std::string& fromNodeName, const std::string& toNodeName, double current);       // <-- اضافه شده


    Node* getNode(const std::string& name) const;
    const std::vector<CircuitElement*>& getElements() const;
    const std::map<std::string, Node*>& getNodes() const;
    void handleCommand(const std::string& input);

    void printCircuitDetails() const;
    void analyzeCircuit();

    Circuit(const Circuit&) = delete;
    Circuit& operator=(const Circuit&) = delete;
};
