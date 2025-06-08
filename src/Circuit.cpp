#include "Circuit.h"
#include "Node.h"
#include "Resistor.h"
#include "Capacitor.h"
#include "Inductor.h"
#include "VoltageSource.h"
#include "CurrentSource.h"
#include "IdealDiode.h"
#include "ZenerDiode.h"
#include <iostream>
#include <regex>
#include <stdexcept>
#include <algorithm>

using namespace std;double parseValueWithSuffix(const std::string& raw) {
    static const std::regex pattern(R"(^([+-]?[0-9]*\.?[0-9]+(?:[eE][+-]?[0-9]+)?)([kKmMuUnN]?(?:[eE][gG])?)$)");
    std::smatch match;

    if (!std::regex_match(raw, match, pattern)) {
        throw std::invalid_argument("Error: Syntax error");
    }

    double baseValue = std::stod(match[1].str());
    std::string suffix = match[2].str();

    // Convert suffix to lowercase for uniformity
    std::transform(suffix.begin(), suffix.end(), suffix.begin(), ::tolower);

    double multiplier = 1.0;
    if (suffix == "k") multiplier = 1e3;
    else if (suffix == "m") multiplier = 1e-3;
    else if (suffix == "u") multiplier = 1e-6;
    else if (suffix == "n") multiplier = 1e-9;
    else if (suffix == "meg") multiplier = 1e6;
    else if (suffix.empty()) multiplier = 1.0;
    else throw std::invalid_argument("Error: Invalid suffix");

    return baseValue * multiplier;
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

Diode* Circuit::addDiode(const std::string& name, const std::string& anodeName, const std::string& cathodeName) {
    Node* n_anode = getOrCreateNode(anodeName);
    Node* n_cathode = getOrCreateNode(cathodeName);
    Diode* diode = new Diode(name, n_anode, n_cathode);
    allElements.push_back(diode);
    return diode;
}

ZenerDiode* Circuit::addZenerDiode(const std::string& name, const std::string& anodeName, const std::string& cathodeName) {
    Node* n_anode = getOrCreateNode(anodeName);
    Node* n_cathode = getOrCreateNode(cathodeName);
    ZenerDiode* zener = new ZenerDiode(name, n_anode, n_cathode);
    allElements.push_back(zener);
    return zener;
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
    std::regex addResistorRegex(R"(add\s+R([A-Za-z0-9_]+)\s+(\w+)\s+(\w+)\s+([+-]?[0-9]*\.?[0-9]+(?:[eE][+-]?[0-9]+)?)([kKmM]|[Mm]eg)?)");
    std::regex addCapacitorRegex(R"(add\s+C([A-Za-z0-9_]+)\s+(\w+)\s+(\w+)\s+([+-]?[0-9]*\.?[0-9]+(?:[eE][+-]?[0-9]+)?[a-zA-Z]*)$)");
    std::regex addInductorRegex(R"(add\s+L([A-Za-z0-9_]+)\s+(\w+)\s+(\w+)\s+([0-9.eE+-]+)([mun]?)$)");
    std::regex addDiodeRegex(R"(add\s+(D\w+)\s+(\w+)\s+(\w+)\s+(\w+))");
    std::regex deleteResistorRegex(R"(delete\s+R([A-Za-z0-9_]+))");
    std::regex deleteCapacitorRegex(R"(delete\s+C([A-Za-z0-9_]+))");
    std::regex deleteInductorRegex(R"(delete\s+L([A-Za-z0-9_]+))");
    std::regex deleteDiodeRegex(R"(delete\s+(D\w+))");

    std::smatch match;

    // --- Add Resistor ---
    if (std::regex_match(input, match, addResistorRegex)) {
        string name = "R" + match[1].str();
        string n1 = match[2].str();
        string n2 = match[3].str();
        string valueStr = match[4].str();

        double resistance;
        try {
            resistance = parseValueWithSuffix(valueStr);  // Use helper for parsing
        } catch (const std::invalid_argument& e) {
            cout << e.what() << endl;
            return;
        }

        if (resistance <= 0) {
            cout << "Error: Resistance cannot be zero or negative" << endl;
            return;
        }

        // Check for duplicate resistor
        for (auto* elem : allElements) {
            if (elem->getName() == name) {
                cout << "Error: Resistor " << match[1].str() << " already exists in the circuit" << endl;
                return;
            }
        }

        addResistor(name, n1, n2, resistance);
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
    //add capacitor
    if (std::regex_match(input, match, addCapacitorRegex)) {
        std::string name = "C" + match[1].str();
        std::string n1 = match[2].str();
        std::string n2 = match[3].str();
        std::string valueStr = match[4].str();

        // Check if capacitor already exists by scanning allElements
        for (auto* elem : allElements) {
            if (elem->getName() == name) {
                std::cout << "Error: Capacitor " << match[1].str() << " already exists in the circuit\n";
                return;
            }
        }

        double capacitance = 0.0;
        try {
            capacitance = parseValueWithSuffix(valueStr);
        } catch (const std::invalid_argument& e) {
            std::cout << e.what() << "\n";
            return;
        }

        if (capacitance <= 0) {
            std::cout << "Error: Capacitance cannot be zero or negative\n";
            return;
        }

        addCapacitor(name, n1, n2, capacitance);

        std::cout << "Capacitor '" << name << "' added between " << n1 << " and " << n2 << " with value " << capacitance << " F.\n";
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

    // --- Add Inductor ---
    if (std::regex_match(input, match, addInductorRegex)) {
        std::string name = "L" + match[1].str();
        std::string n1 = match[2].str();
        std::string n2 = match[3].str();
        std::string rawValue = match[4].str() + match[5].str();  // number + unit suffix together

        // Check if inductor already exists
        for (auto* e : allElements) {
            if (e->getName() == name) {
                std::cout << "Error: Inductor " << name << " already exists in the circuit\n";
                return;
            }
        }

        double inductance = 0.0;
        try {
            inductance = parseValueWithSuffix(rawValue);
        } catch (const std::invalid_argument& e) {
            std::cout << e.what() << "\n";
            return;
        }

        if (inductance <= 0) {
            std::cout << "Error: Inductance cannot be zero or negative\n";
            return;
        }

        addInductor(name, n1, n2, inductance);
        std::cout << "Inductor '" << name << "' added between " << n1 << " and " << n2 << " with value " << inductance << " H.\n";
        return;
    }


    // --- Delete Inductor ---
    if (std::regex_match(input, match, deleteInductorRegex)) {
        string name = "L" + match[1].str();

        auto it = std::remove_if(allElements.begin(), allElements.end(), [&](CircuitElement* e) {
            if (e->getName() == name && e->getElementType() == ElementType::INDUCTOR) {
                delete e;
                return true;
            }
            return false;
        });

        if (it == allElements.end()) {
            cout << "Error: Cannot delete inductor; component not found\n";
        } else {
            allElements.erase(it, allElements.end());
            cout << "Inductor " << name << " deleted\n";
        }
        return;
    }
    // --- Add GND Node ---
    if (regex_match(input, match, regex(R"(add\s+GND\s+(\w+))"))) {
        string nodeName = match[1];

        if (input.substr(4, 3) != "GND") {
            cerr << "Error: Element " << nodeName << " not found in library" << endl;
            return;
        }

        Node* node = getNode(nodeName);
        if (!node) {
            cerr << "Node does not exist" << endl;
            return;
        }

        groundedNodes.insert(nodeName);
        cout << "Ground added to node '" << nodeName << "'." << endl;
        return;
    }
    // // --- delete GND Node ---
    if (regex_match(input, match, regex(R"(delete\s+GND\s+(\w+))"))) {
        string nodeName = match[1];

        if (input.substr(7, 3) != "GND") {
            cerr << "Error: Element " << nodeName << " not found in library" << endl;
            return;
        }

        if (groundedNodes.find(nodeName) == groundedNodes.end()) {
            cerr << "Node does not exist" << endl;
            return;
        }

        groundedNodes.erase(nodeName);
        cout << "Ground removed from node '" << nodeName << "'." << endl;
        return;
    }

    // --- Add Diode ---
    if (std::regex_match(input, match, addDiodeRegex)) {
        string name = match[1].str();
        string anode = match[2].str();
        string cathode = match[3].str();
        string model = match[4].str();

        for (auto* elem : allElements) {
            if (elem->getName() == name) {
                cout << "Error: Diode " << name << " already exists in the circuit" << endl;
                return;
            }
        }

        if (model == "D") {
            addDiode(name, anode, cathode);
        } else if (model == "Z") {
            addZenerDiode(name, anode, cathode);
        } else {
            cout << "Error: Model " << model << " not found in library" << endl;
        }
        return;
    }

    // --- Delete Diode ---
    if (std::regex_match(input, match, deleteDiodeRegex)) {
        string name = match[1].str();

        auto it = std::remove_if(allElements.begin(), allElements.end(), [&](CircuitElement* e) {
            if (e->getName() == name && e->getElementType() == ElementType::DIODE) {
                delete e;
                return true;
            }
            return false;
        });

        if (it == allElements.end()) {
            cout << "Error: Cannot delete diode; component not found\n";
        } else {
            allElements.erase(it, allElements.end());
            cout << "Diode " << name << " deleted\n";
        }
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
