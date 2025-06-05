#pragma once

#include <string>
#include <vector>
#include <map>
#include <stdexcept> // For std::runtime_error

// Forward declarations
class Node;
class CircuitElement;
class Resistor;
class Capacitor;
class Inductor;

class Circuit {
private:
    std::map<std::string, Node*> namedNodes;
    std::vector<CircuitElement*> allElements;
    int nextNodeId; // To assign unique IDs to nodes

    // Helper to get or create a node
    Node* getOrCreateNode(const std::string& nodeName);

public:
    Circuit();
    ~Circuit();

    // Methods to add elements
    // Returns raw pointer, Circuit class owns the memory
    Node* addNode(const std::string& name); // Explicitly add a node if needed

    Resistor* addResistor(const std::string& name, const std::string& node1Name, const std::string& node2Name, double resistance);
    Capacitor* addCapacitor(const std::string& name, const std::string& node1Name, const std::string& node2Name, double capacitance);
    Inductor* addInductor(const std::string& name, const std::string& node1Name, const std::string& node2Name, double inductance);
    // Add methods for other element types (VoltageSource, CurrentSource, etc.)

    // Getters
    Node* getNode(const std::string& name) const;
    const std::vector<CircuitElement*>& getElements() const;
    const std::map<std::string, Node*>& getNodes() const;

    // Circuit operations
    void printCircuitDetails() const;
    void analyzeCircuit(); // Placeholder for analysis logic

    // Prevent copying and assignment if not properly handled
    Circuit(const Circuit&) = delete;
    Circuit& operator=(const Circuit&) = delete;
};
