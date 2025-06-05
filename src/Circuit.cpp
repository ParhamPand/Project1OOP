#include "Circuit.h"
#include "Node.h"         //
#include "Resistor.h"     //
#include "Capacitor.h"    // Assumes Capacitor.h is created as per previous step
#include "Inductor.h"     // Assumes Inductor.h is created as per previous step
#include <iostream>
#include <algorithm> // For std::find_if if needed, though map lookup is better

using namespace std;

Circuit::Circuit() : nextNodeId(0) {
    cout << "Circuit created." << endl;
    // Ensure ground node "0" or "gnd" often exists by default or is handled.
    // For simplicity, we'll let users add all nodes including ground.
}

Circuit::~Circuit() {
    cout << "Destroying Circuit..." << endl;
    // Delete all elements
    for (CircuitElement* elem : allElements) {
        delete elem;
    }
    allElements.clear();

    // Delete all nodes
    for (auto const& [name, node_ptr] : namedNodes) {
        delete node_ptr;
    }
    namedNodes.clear();
    cout << "Circuit destroyed." << endl;
}

Node* Circuit::getOrCreateNode(const std::string& nodeName) {
    if (namedNodes.find(nodeName) == namedNodes.end()) {
        // Node doesn't exist, create it
        Node* newNode = new Node(nextNodeId++, nodeName); //
        namedNodes[nodeName] = newNode;
        cout << "Node '" << nodeName << "' (ID: " << newNode->getId() << ") created and added to circuit." << endl; //
        return newNode;
    }
    return namedNodes.at(nodeName);
}

Node* Circuit::addNode(const std::string& name) {
    if (namedNodes.count(name)) {
        cerr << "Warning: Node with name '" << name << "' already exists. Returning existing node." << endl;
        return namedNodes.at(name);
    }
    Node* newNode = new Node(nextNodeId++, name); //
    namedNodes[name] = newNode;
    cout << "Node '" << name << "' (ID: " << newNode->getId() << ") explicitly added to circuit." << endl; //
    return newNode;
}


Resistor* Circuit::addResistor(const std::string& name, const std::string& node1Name, const std::string& node2Name, double resistance) {
    Node* n1 = getOrCreateNode(node1Name);
    Node* n2 = getOrCreateNode(node2Name);
    Resistor* res = new Resistor(name, n1, n2, resistance); //
    allElements.push_back(res);
    cout << "Resistor '" << name << "' added between " << node1Name << " and " << node2Name << "." << endl;
    return res;
}

Capacitor* Circuit::addCapacitor(const std::string& name, const std::string& node1Name, const std::string& node2Name, double capacitance) {
    Node* n1 = getOrCreateNode(node1Name);
    Node* n2 = getOrCreateNode(node2Name);
    Capacitor* cap = new Capacitor(name, n1, n2, capacitance); // Assumes Capacitor constructor is similar to Resistor's
    allElements.push_back(cap);
    cout << "Capacitor '" << name << "' added between " << node1Name << " and " << node2Name << "." << endl;
    return cap;
}

Inductor* Circuit::addInductor(const std::string& name, const std::string& node1Name, const std::string& node2Name, double inductance) {
    Node* n1 = getOrCreateNode(node1Name);
    Node* n2 = getOrCreateNode(node2Name);
    Inductor* ind = new Inductor(name, n1, n2, inductance); // Assumes Inductor constructor is similar to Resistor's
    allElements.push_back(ind);
    cout << "Inductor '" << name << "' added between " << node1Name << " and " << node2Name << "." << endl;
    return ind;
}

Node* Circuit::getNode(const std::string& name) const {
    auto it = namedNodes.find(name);
    if (it != namedNodes.end()) {
        return it->second;
    }
    return nullptr; // Or throw an exception
}

const std::vector<CircuitElement*>& Circuit::getElements() const {
    return allElements;
}

const std::map<std::string, Node*>& Circuit::getNodes() const {
    return namedNodes;
}

void Circuit::printCircuitDetails() const {
    cout << "\n--- Circuit Details ---" << endl;
    cout << "Nodes (" << namedNodes.size() << "):" << endl;
    for (const auto& pair : namedNodes) {
        cout << "  - Node Name: " << pair.first << ", ID: " << pair.second->getId() << endl; //
    }

    cout << "\nElements (" << allElements.size() << "):" << endl;
    for (const auto* elem : allElements) {
        cout << "  - ";
        elem->printDetails(); //
    }
    cout << "--- End of Circuit Details ---" << endl;
}

void Circuit::analyzeCircuit() {
    // Placeholder for actual circuit analysis (e.g., MNA setup and solution)
    cout << "\n--- Circuit Analysis ---" << endl;
    if (allElements.empty()) {
        cout << "Circuit is empty. Nothing to analyze." << endl;
        return;
    }

    cout << "Preparing for analysis..." << endl;
    cout << "Number of nodes (excluding potential ground if implicit): " << namedNodes.size() << endl;
    cout << "Number of elements: " << allElements.size() << endl;

    // Here you would:
    // 1. Determine the size of the MNA matrix (number of nodes - 1 + number of voltage-defined elements)
    // 2. Create the matrix and RHS vector
    // 3. Iterate through all elements and apply their stamps
    //    for (const auto* elem : allElements) {
    //        elem->applyStamps(/* matrix, rhs */);
    //    }
    // 4. Solve the system of equations
    // 5. Store/display results (node voltages, branch currents)

    cout << "Applying stamps for all elements (placeholder):" << endl;
    for (const auto* elem : allElements) {
        elem->applyStamps(/* placeholder for actual matrix */); //
    }

    cout << "Circuit analysis placeholder complete. Matrix solving would happen here." << endl;
    cout << "--- End of Analysis ---" << endl;
}
