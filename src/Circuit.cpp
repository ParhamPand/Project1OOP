#include "Circuit.h"
#include "Node.h"
#include "Resistor.h"
#include "Capacitor.h"
#include "Inductor.h"
#include "VoltageSource.h"
#include "CurrentSource.h"
#include <iostream>
#include <regex>
#include <algorithm>

using namespace std;
double parseResistanceValue(const std::string& str) {
    try {
        size_t idx = 0;
        double base = std::stod(str, &idx);
        std::string suffix = str.substr(idx);

        if (suffix == "k" || suffix == "K")
            return base * 1e3;
        if (suffix == "M" || suffix == "Meg" || suffix == "meg")
            return base * 1e6;
        if (suffix.empty())
            return base;

        throw std::invalid_argument("Invalid suffix");
    } catch (...) {
        throw std::invalid_argument("Error: Syntax error");
    }
}

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

//    // Delete all nodes
//    for (auto const& [name, node_ptr] : namedNodes) {
//        delete node_ptr;
//    }
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

VoltageSource* Circuit::addVoltageSource(const std::string& name, const std::string& positiveNodeName, const std::string& negativeNodeName, double voltage) {
    Node* n_pos = getOrCreateNode(positiveNodeName);
    Node* n_neg = getOrCreateNode(negativeNodeName);
    VoltageSource* v_src = new VoltageSource(name, n_pos, n_neg, voltage);
    allElements.push_back(v_src);
    cout << "VoltageSource '" << name << "' added. Positive: " << positiveNodeName << ", Negative: " << negativeNodeName << "." << endl;
    return v_src;
}

CurrentSource* Circuit::addCurrentSource(const std::string& name, const std::string& fromNodeName, const std::string& toNodeName, double current) {
    Node* n_from = getOrCreateNode(fromNodeName);
    Node* n_to = getOrCreateNode(toNodeName);
    CurrentSource* i_src = new CurrentSource(name, n_from, n_to, current);
    allElements.push_back(i_src);
    cout << "CurrentSource '" << name << "' added. From: " << fromNodeName << ", To: " << toNodeName << " (through external circuit)." << endl;
    return i_src;
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
void Circuit::handleCommand(const std::string& input) {
    // Regex for add commands (resistor and capacitor)
    std::regex addResistorRegex(R"(add\s+R([A-Za-z0-9_]+)\s+(\w+)\s+(\w+)\s+([0-9.eE+-]+)([kKmM]eg?)?)");
    std::regex addCapacitorRegex(R"(add\s+C([A-Za-z0-9_]+)\s+(\w+)\s+(\w+)\s+([0-9.eE+-]+)(nF|uF|F)?)");
    std::regex deleteResistorRegex(R"(delete\s+R([A-Za-z0-9_]+))");
    std::regex deleteCapacitorRegex(R"(delete\s+C([A-Za-z0-9_]+))");

    std::smatch match;

    // --- Add Resistor ---
    if (std::regex_match(input, match, addResistorRegex)) {
        string name = "R" + match[1].str();
        string n1 = match[2].str();
        string n2 = match[3].str();
        string valueStr = match[4].str();
        string suffix = match[5].str();

        // Parse resistor value with suffix
        double multiplier = 1.0;
        if (suffix == "k" || suffix == "K") multiplier = 1e3;
        else if (suffix == "M" || suffix == "Meg" || suffix == "meg") multiplier = 1e6;

        double value;
        try {
            value = std::stod(valueStr) * multiplier;
        } catch (...) {
            cout << "Error: Invalid resistance value\n";
            return;
        }

        if (value <= 0) {
            cout << "Error: Resistance cannot be zero or negative\n";
            return;
        }

        // Check if resistor already exists
        for (auto* e : allElements) {
            if (e->getName() == name) {
                cout << "Error: Resistor " << name << " already exists in the circuit\n";
                return;
            }
        }

        addResistor(name, n1, n2, value);
        return;
    }

    // --- Delete Resistor ---
    if (std::regex_match(input, match, deleteResistorRegex)) {
        string name = "R" + match[1].str();

        auto it = std::remove_if(allElements.begin(), allElements.end(), [&](CircuitElement* e) {
            if (e->getName() == name && e->getElementType() == ElementType::RESISTOR) {
                delete e;
                return true;
            }
            return false;
        });

        if (it == allElements.end()) {
            cout << "Error: Cannot delete resistor; component not found\n";
        } else {
            allElements.erase(it, allElements.end());
            cout << "Resistor " << name << " deleted\n";
        }
        return;
    }

    // --- Add Capacitor ---
    if (std::regex_match(input, match, addCapacitorRegex)) {
        string name = "C" + match[1].str();
        string n1 = match[2].str();
        string n2 = match[3].str();
        string valueStr = match[4].str();
        string unit = match[5].str();

        double multiplier = 1.0;
        if (unit == "nF") multiplier = 1e-9;
        else if (unit == "uF") multiplier = 1e-6;
        else if (unit == "F") multiplier = 1.0;
        else if (!unit.empty()) {
            cout << "Error: Unknown unit for capacitance\n";
            return;
        }

        double value;
        try {
            value = std::stod(valueStr) * multiplier;
        } catch (...) {
            cout << "Error: Invalid capacitance value\n";
            return;
        }

        if (value <= 0) {
            cout << "Error: Capacitance cannot be zero or negative\n";
            return;
        }

        // Check for duplicate
        for (auto* e : allElements) {
            if (e->getName() == name) {
                cout << "Error: Capacitor " << name << " already exists in the circuit\n";
                return;
            }
        }

        addCapacitor(name, n1, n2, value);
        return;
    }

    // --- Delete Capacitor ---
    if (std::regex_match(input, match, deleteCapacitorRegex)) {
        string name = "C" + match[1].str();

        auto it = std::remove_if(allElements.begin(), allElements.end(), [&](CircuitElement* e) {
            if (e->getName() == name && e->getElementType() == ElementType::CAPACITOR) {
                delete e;
                return true;
            }
            return false;
        });

        if (it == allElements.end()) {
            cout << "Error: Cannot delete capacitor; component not found\n";
        } else {
            allElements.erase(it, allElements.end());
            cout << "Capacitor " << name << " deleted\n";
        }
        return;
    }

    // --- Error: wrong element library (e.g. lowercase r, c, etc.) ---
    if (std::regex_match(input, std::regex(R"(add\s+([a-z])\w*)"))) {
        cout << "Error: Element " << input.substr(4, input.find(' ', 4) - 4) << " not found in library\n";
        return;
    }

    // --- Generic syntax error ---
    cout << "Error: Syntax error\n";
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

    cout << "Applying stamps for all elements (placeholder):" << endl;
    for (const auto* elem : allElements) {
        elem->applyStamps(/* placeholder for actual matrix */); //
    }

    cout << "Circuit analysis placeholder complete. Matrix solving would happen here." << endl;
    cout << "--- End of Analysis ---" << endl;
}
