#include "Circuit.h"
#include "Node.h"
#include "Resistor.h"
#include "Capacitor.h"
#include "Inductor.h"
#include "VoltageSource.h"
#include "CurrentSource.h"
#include "IdealDiode.h"
#include "ZenerDiode.h"
#include "SinusoidalVoltageSource.h"
#include "MatrixSolver.h"
#include <iostream>
#include <regex>
#include <stdexcept>
#include <algorithm>
#include <sstream>

using namespace std;

double parseValueWithSuffix(const string& raw) {
    static const regex pattern(R"(^([+-]?[0-9]*\.?[0-9]+(?:[eE][+-]?[0-9]+)?)([kKmMuUnN]?(?:[eE][gG])?)$)");
    smatch match;

    if (!regex_match(raw, match, pattern)) {
        throw invalid_argument("Error: Syntax error");
    }

    double baseValue = stod(match[1].str());
    string suffix = match[2].str();

    // Convert suffix to lowercase for uniformity
    transform(suffix.begin(), suffix.end(), suffix.begin(), ::tolower);

    double multiplier = 1.0;
    if (suffix == "k") multiplier = 1e3;
    else if (suffix == "m") multiplier = 1e-3;
    else if (suffix == "u") multiplier = 1e-6;
    else if (suffix == "n") multiplier = 1e-9;
    else if (suffix == "meg") multiplier = 1e6;
    else if (suffix.empty()) multiplier = 1.0;
    else throw invalid_argument("Error: Invalid suffix");

    return baseValue * multiplier;
}

Circuit::Circuit() : nextNodeId(0) , mna_extra_vars_count(0) {
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

Node* Circuit::getOrCreateNode(const string& nodeName) {
    if (namedNodes.find(nodeName) == namedNodes.end()) {
        // Node doesn't exist, create it
        Node* newNode = new Node(nextNodeId++, nodeName); //
        namedNodes[nodeName] = newNode;
        cout << "Node '" << nodeName << "' (ID: " << newNode->getId() << ") created and added to circuit." << endl; //
        return newNode;
    }
    return namedNodes.at(nodeName);
}

Node* Circuit::addNode(const string& name) {
    if (namedNodes.count(name)) {
        cerr << "Warning: Node with name '" << name << "' already exists. Returning existing node." << endl;
        return namedNodes.at(name);
    }
    Node* newNode = new Node(nextNodeId++, name); //
    namedNodes[name] = newNode;
    cout << "Node '" << name << "' (ID: " << newNode->getId() << ") explicitly added to circuit." << endl; //
    return newNode;
}


Resistor* Circuit::addResistor(const string& name, const string& node1Name, const string& node2Name, double resistance) {
    Node* n1 = getOrCreateNode(node1Name);
    Node* n2 = getOrCreateNode(node2Name);
    Resistor* res = new Resistor(name, n1, n2, resistance);
    allElements.push_back(res);
    cout << "Resistor '" << name << "' with resistance " << resistance << " Ohms added between " << node1Name << " and " << node2Name << "." << endl;
    return res;
}

Capacitor* Circuit::addCapacitor(const string& name, const string& node1Name, const string& node2Name, double capacitance) {
    Node* n1 = getOrCreateNode(node1Name);
    Node* n2 = getOrCreateNode(node2Name);
    Capacitor* cap = new Capacitor(name, n1, n2, capacitance); // Assumes Capacitor constructor is similar to Resistor's
    allElements.push_back(cap);
    cout << "Capacitor '" << name << "' added between " << node1Name << " and " << node2Name << "." << endl;
    return cap;
}

Inductor* Circuit::addInductor(const string& name, const string& node1Name, const string& node2Name, double inductance) {
    Node* n1 = getOrCreateNode(node1Name);
    Node* n2 = getOrCreateNode(node2Name);
    Inductor* ind = new Inductor(name, n1, n2, inductance); // Assumes Inductor constructor is similar to Resistor's

    CircuitElement* elem = ind;
    elem->mna_branch_index = mna_extra_vars_count++;

    allElements.push_back(ind);
    cout << "Inductor '" << name << "' added between " << node1Name << " and " << node2Name << "." << endl;
    return ind;
}

VoltageSource* Circuit::addVoltageSource(const string& name, const string& positiveNodeName, const string& negativeNodeName, double voltage) {
    Node* n_pos = getOrCreateNode(positiveNodeName);
    Node* n_neg = getOrCreateNode(negativeNodeName);
    VoltageSource* v_src = new VoltageSource(name, n_pos, n_neg, voltage);

    CircuitElement* elem = v_src;
    elem->mna_branch_index = mna_extra_vars_count++;

    allElements.push_back(v_src);
    cout << "VoltageSource '" << name << "' with value " << voltage << "V added. Positive: " << positiveNodeName << ", Negative: " << negativeNodeName << "." << endl;
    return v_src;
}

CurrentSource* Circuit::addCurrentSource(const string& name, const string& fromNodeName, const string& toNodeName, double current) {
    Node* n_from = getOrCreateNode(fromNodeName);
    Node* n_to = getOrCreateNode(toNodeName);
    CurrentSource* i_src = new CurrentSource(name, n_from, n_to, current);
    allElements.push_back(i_src);
    cout << "CurrentSource '" << name << "' with value " << current << "A added. From: " << fromNodeName << ", To: " << toNodeName << " (through external circuit)." << endl;
    return i_src;
}

Diode* Circuit::addDiode(const string& name, const string& anodeName, const string& cathodeName) {
    Node* n_anode = getOrCreateNode(anodeName);
    Node* n_cathode = getOrCreateNode(cathodeName);
    Diode* diode = new Diode(name, n_anode, n_cathode);
    allElements.push_back(diode);
    return diode;
}

ZenerDiode* Circuit::addZenerDiode(const string& name, const string& anodeName, const string& cathodeName) {
    Node* n_anode = getOrCreateNode(anodeName);
    Node* n_cathode = getOrCreateNode(cathodeName);
    ZenerDiode* zener = new ZenerDiode(name, n_anode, n_cathode);
    allElements.push_back(zener);
    return zener;
}

SinusoidalVoltageSource* Circuit::addSinusoidalVoltageSource(const string& name, const string& node1Name, const string& node2Name, double offset, double amplitude, double frequency) {
    Node* n1 = getOrCreateNode(node1Name);
    Node* n2 = getOrCreateNode(node2Name);
    auto* src = new SinusoidalVoltageSource(name, n1, n2, offset, amplitude, frequency);
    allElements.push_back(src);
    cout << "SinusoidalVoltageSource '" << name << "' added." << endl;
    return src;
}



Node* Circuit::getNode(const string& name) const {
    auto it = namedNodes.find(name);
    if (it != namedNodes.end()) {
        return it->second;
    }
    return nullptr; // Or throw an exception
}

const vector<CircuitElement*>& Circuit::getElements() const {
    return allElements;
}

const map<string, Node*>& Circuit::getNodes() const {
    return namedNodes;
}


void Circuit::performVoltageTransientAnalysis(double t_step, double t_stop, const string& output_node_name) {
    // 1. Validation Checks
    if (t_step <= 0 || t_stop <= 0 || t_stop < t_step) {
        cout << "Error: Invalid time step or stop time." << endl;
        return;
    }
    if (groundedNodes.empty()) {
        cout << "Error: No ground node specified. Analysis cannot proceed." << endl;
        return;
    }
    if (namedNodes.find(output_node_name) == namedNodes.end()) {
        cout << "Error: Output node '" << output_node_name << "' not found." << endl;
        return;
    }

    // 2. MNA System Initialization
    map<string, int> node_map;
    int node_count = 0;
    for (const auto& pair : namedNodes) {
        if (groundedNodes.find(pair.first) == groundedNodes.end()) {
            node_map[pair.first] = node_count++;
        }
    }

    int system_size = node_count + mna_extra_vars_count;
    vector<double> x_prev(system_size, 0.0); // Solution vector from previous time step

    int output_node_index = (output_node_name == *groundedNodes.begin() || output_node_name == "0")
                            ? -1
                            : node_map.at(output_node_name);

    // 3. Time-Stepping Loop
    cout << "Time\tV(" << output_node_name << ")" << endl;
    for (double t = t_step; t <= t_stop; t += t_step) {
        // a. Create and reset MNA matrices for the current time step
        vector<vector<double>> A(system_size, vector<double>(system_size, 0.0));
        vector<double> b(system_size, 0.0);

        // b. Apply stamps from all elements
        for (const auto* elem : allElements) {
            elem->applyStamps(A, b, node_map, x_prev, node_count, t, t_step);
        }

        try {
            // c. Solve the system Ax = b
            MatrixSolver solver(A, b);
            vector<double> x_current = solver.solve();

            // d. Get and print the output voltage
            double output_voltage = (output_node_index != -1) ? x_current[output_node_index] : 0.0;
            cout << t << "\t" << output_voltage << endl;

            // e. Update the state for the next iteration
            x_prev = x_current;

        } catch (const runtime_error& e) {
            cout << "Error during simulation at t = " << t << ": " << e.what() << endl;
            return; // Stop the analysis on solver failure
        }
    }
}

void Circuit::performCurrentTransientAnalysis(double t_step, double t_stop, const string& output_element_name) {
    // --- Validation Section ---
    if (t_step <= 0 || t_stop <= 0 || t_stop < t_step) {
        cout << "Error: Invalid time step or stop time." << endl;
        return;
    }

    // این شرط را برای بررسی گره زمین اضافه کنید
    if (groundedNodes.empty()) {
        cout << "Error: No ground node defined. Analysis is not possible." << endl;
        return;
    }

    CircuitElement* target_element = getElement(output_element_name);
    if (!target_element) {
        cout << "Error: Element '" << output_element_name << "' not found." << endl;
        return;
    }

    // MNA Setup
    map<string, int> node_map;
    int node_count = 0;
    for (const auto& pair : namedNodes) {
        if (groundedNodes.find(pair.first) == groundedNodes.end()) {
            node_map[pair.first] = node_count++;
        }
    }

    int system_size = node_count + mna_extra_vars_count;
    vector<double> x_prev(system_size, 0.0);


    // --- Calculate Initial State at t=0 (without printing) ---
    vector<vector<double>> A_dc(system_size, vector<double>(system_size, 0.0));
    vector<double> b_dc(system_size, 0.0);
    for (const auto* elem : allElements) {
        elem->applyStamps(A_dc, b_dc, node_map, vector<double>(system_size, 0.0), node_count, 0.0, t_step);
    }
    try {
        MatrixSolver dc_solver(A_dc, b_dc);
        x_prev = dc_solver.solve(); // x_prev now holds the state at t=0
    } catch (const runtime_error& e) {
        cout << "Error during initial DC analysis for current: " << e.what() << endl;
        return;
    }
    // Print Header
    cout << "Time\tI(" << output_element_name << ")" << endl;

    // Time-Stepping Loop
    for (double t = t_step ; t <= t_stop; t += t_step) {
        vector<vector<double>> A(system_size, vector<double>(system_size, 0.0));
        vector<double> b(system_size, 0.0);

        for (const auto* elem : allElements) {
            elem->applyStamps(A, b, node_map, x_prev, node_count, t, t_step);
        }

        try {
            MatrixSolver solver(A, b);
            vector<double> x_current = solver.solve();

            double output_current = 0.0;
            // --- Current Calculation Logic ---
            Node* n1 = target_element->getNode1();
            Node* n2 = target_element->getNode2();
            int n1_idx = (n1 && node_map.count(n1->getName())) ? node_map.at(n1->getName()) : -1;
            int n2_idx = (n2 && node_map.count(n2->getName())) ? node_map.at(n2->getName()) : -1;

            double v1 = (n1_idx != -1) ? x_current[n1_idx] : 0.0;
            double v2 = (n2_idx != -1) ? x_current[n2_idx] : 0.0;

            ElementType type = target_element->getElementType();
            double value = target_element->getValue();

            if (type == ElementType::VOLTAGE_SOURCE || type == ElementType::SINUSOIDAL_VOLTAGE_SOURCE || type == ElementType::INDUCTOR) {
                output_current = x_current[node_count + target_element->mna_branch_index];
            } else if (type == ElementType::RESISTOR) {
                output_current = (value > 1e-9) ? (v1 - v2) / value : 0.0;
            } else if (type == ElementType::CAPACITOR) {
                double v1_prev = (n1_idx != -1 && n1_idx < x_prev.size()) ? x_prev[n1_idx] : 0.0;
                double v2_prev = (n2_idx != -1 && n2_idx < x_prev.size()) ? x_prev[n2_idx] : 0.0;
                output_current = (t_step > 1e-12) ? value * ((v1 - v2) - (v1_prev - v2_prev)) / t_step : 0.0;
            } else if (type == ElementType::CURRENT_SOURCE) {
                output_current = value;
            }

            cout << t << "\t" << output_current << endl;
            x_prev = x_current;

        } catch (const runtime_error& e) {
            cout << "Error during simulation at t = " << t << ": " << e.what() << endl;
            return;
        }
    }
}

CircuitElement* Circuit::getElement(const string& name) const {
    for (auto* elem : allElements) {
        if (elem->getName() == name) {
            return elem;
        }
    }
    return nullptr;
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

void Circuit::printNodes() const {
    if (namedNodes.empty()) {
        cout << "There are no nodes in the circuit." << endl;
        return;
    }

    cout << "Available nodes:" << endl;

    auto it = namedNodes.cbegin();
    cout << it->first;
    ++it;
    for (; it != namedNodes.cend(); ++it) {
        cout << ", " << it->first;
    }
    cout << endl;
}


void Circuit::handleCommand(const string& input) {
    // Regex for list commands
    regex nodesRegex(R"(\s*\.nodes\s*)");
    // Regex for add commands
    regex addResistorRegex(R"(add\s+R([A-Za-z0-9_]+)\s+(\w+)\s+(\w+)\s+([+-]?[0-9]*\.?[0-9]+(?:[eE][+-]?[0-9]+)?[a-zA-Z]*)$)");
    regex addCapacitorRegex(R"(add\s+C([A-Za-z0-9_]+)\s+(\w+)\s+(\w+)\s+([+-]?[0-9]*\.?[0-9]+(?:[eE][+-]?[0-9]+)?[a-zA-Z]*)$)");
    regex addInductorRegex(R"(add\s+L([A-Za-z0-9_]+)\s+(\w+)\s+(\w+)\s+([0-9.eE+-]+)([mun]?)$)");
    regex addDiodeRegex(R"(add\s+(D\w+)\s+(\w+)\s+(\w+)\s+(\w+))");
    regex addVoltageSourceRegex(R"(add\s+VoltageSource([A-Za-z0-9_]+)\s+(\w+)\s+(\w+)\s+(.+)$)");
    regex addSinSourceRegex(R"(add\s+V([A-Za-z0-9_]+)\s+(\w+)\s+(\w+)\s+SIN\s*\(\s*([+-]?[0-9]*\.?[0-9]+(?:[eE][+-]?[0-9]+)?)\s+([+-]?[0-9]*\.?[0-9]+(?:[eE][+-]?[0-9]+)?)\s+([+-]?[0-9]*\.?[0-9]+(?:[eE][+-]?[0-9]+)?)\s*\))");
    regex addCurrentSourceRegex(R"(add\s+CurrentSource([A-Za-z0-9_]+)\s+(\w+)\s+(\w+)\s+(.+)$)");
    regex deleteResistorRegex(R"(delete\s+R([A-Za-z0-9_]+))");
    regex deleteCapacitorRegex(R"(delete\s+C([A-Za-z0-9_]+))");
    regex deleteInductorRegex(R"(delete\s+L([A-Za-z0-9_]+))");
    regex deleteDiodeRegex(R"(delete\s+(D\w+))");
    // Regex for Circuit analysis
    regex tranVoltageRegex(R"(\s*\.print\s+TRAN\s+([0-9.eE+-]+)\s+([0-9.eE+-]+)\s+V\((\w+)\)\s*)");
    regex tranCurrentRegex(R"(\s*\.print\s+TRAN\s+([0-9.eE+-]+)\s+([0-9.eE+-]+)\s+I\((\w+)\)\s*)");
    regex tranMultiRegex(R"(\s*\.print\s+TRAN\s+([0-9.eE+-]+)\s+([0-9.eE+-]+)\s+(.*))");


    smatch match;
    // show Nodes
    if (regex_match(input, nodesRegex)) {
        printNodes();
        return;
    }

    // --- Add Resistor ---
    if (regex_match(input, match, addResistorRegex)) {
        string name = "R" + match[1].str();
        string n1 = match[2].str();
        string n2 = match[3].str();
        string valueStr = match[4].str();

        double resistance;
        try {

            resistance = parseValueWithSuffix(valueStr);
        } catch (const invalid_argument& e) {
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
    if (regex_match(input, match, deleteResistorRegex)) {
        string name = "R" + match[1].str();

        auto it = remove_if(allElements.begin(), allElements.end(), [&](CircuitElement* e) {
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
    if (regex_match(input, match, addCurrentSourceRegex)) {
        string name = "CurrentSource" + match[1].str();
        string n_from = match[2].str();
        string n_to = match[3].str();
        string valueStr = match[4].str();

        for (auto* elem : allElements) {
            if (elem->getName() == name) {
                cout << "Error: Component " << name << " already exists in the circuit" << endl;
                return;
            }
        }

        double current;
        try {
            current = parseValueWithSuffix(valueStr);
        } catch (const invalid_argument& e) {
            cout << e.what() << endl;
            return;
        }
        addCurrentSource(name, n_from, n_to, current);
        return;
    }
    //add capacitor
    if (regex_match(input, match, addCapacitorRegex)) {
        string name = "C" + match[1].str();
        string n1 = match[2].str();
        string n2 = match[3].str();
        string valueStr = match[4].str();

        // Check if capacitor already exists by scanning allElements
        for (auto* elem : allElements) {
            if (elem->getName() == name) {
                cout << "Error: Capacitor " << match[1].str() << " already exists in the circuit\n";
                return;
            }
        }

        double capacitance = 0.0;
        try {
            capacitance = parseValueWithSuffix(valueStr);
        } catch (const invalid_argument& e) {
            cout << e.what() << "\n";
            return;
        }

        if (capacitance <= 0) {
            cout << "Error: Capacitance cannot be zero or negative\n";
            return;
        }

        addCapacitor(name, n1, n2, capacitance);

        cout << "Capacitor '" << name << "' added between " << n1 << " and " << n2 << " with value " << capacitance << " F.\n";
        return;
    }

    // --- Delete Capacitor ---
    if (regex_match(input, match, deleteCapacitorRegex)) {
        string name = "C" + match[1].str();

        auto it = remove_if(allElements.begin(), allElements.end(), [&](CircuitElement* e) {
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
    if (regex_match(input, regex(R"(add\s+([a-z])\w*)"))) {
        cout << "Error: Element " << input.substr(4, input.find(' ', 4) - 4) << " not found in library\n";
        return;
    }

    // --- Add Inductor ---
    if (regex_match(input, match, addInductorRegex)) {
        string name = "L" + match[1].str();
        string n1 = match[2].str();
        string n2 = match[3].str();
        string rawValue = match[4].str() + match[5].str();  // number + unit suffix together

        // Check if inductor already exists
        for (auto* e : allElements) {
            if (e->getName() == name) {
                cout << "Error: Inductor " << name << " already exists in the circuit\n";
                return;
            }
        }

        double inductance = 0.0;
        try {
            inductance = parseValueWithSuffix(rawValue);
        } catch (const invalid_argument& e) {
            cout << e.what() << "\n";
            return;
        }

        if (inductance <= 0) {
            cout << "Error: Inductance cannot be zero or negative\n";
            return;
        }

        addInductor(name, n1, n2, inductance);
        cout << "Inductor '" << name << "' added between " << n1 << " and " << n2 << " with value " << inductance << " H.\n";
        return;
    }


    // --- Delete Inductor ---
    if (regex_match(input, match, deleteInductorRegex)) {
        string name = "L" + match[1].str();

        auto it = remove_if(allElements.begin(), allElements.end(), [&](CircuitElement* e) {
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
    if (regex_match(input, match, addDiodeRegex)) {
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
    if (regex_match(input, match, deleteDiodeRegex)) {
        string name = match[1].str();

        auto it = remove_if(allElements.begin(), allElements.end(), [&](CircuitElement* e) {
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

    if (regex_match(input, match, addVoltageSourceRegex)) {
        string name = "VoltageSource" + match[1].str();
        string n_pos = match[2].str();
        string n_neg = match[3].str();
        string valueStr = match[4].str();


        for (auto* elem : allElements) {
            if (elem->getName() == name) {
                cout << "Error: Component " << name << " already exists in the circuit" << endl;
                return;
            }
        }

        double voltage;
        try {
            voltage = parseValueWithSuffix(valueStr);
        } catch (const invalid_argument& e) {
            cout << e.what() << endl;
            return;
        }

        addVoltageSource(name, n_pos, n_neg, voltage);
        return;
    }

    if (regex_match(input, match, addSinSourceRegex)) {
        string name = "V" + match[1].str();
        string n_pos = match[2].str();
        string n_neg = match[3].str();

        try {
            double offset = stod(match[4].str());
            double amplitude = stod(match[5].str());
            double frequency = stod(match[6].str());

            for (auto* elem : allElements) {
                if (elem->getName() == name) {
                    cout << "Error: Component " << name << " already exists in the circuit" << endl;
                    return;
                }
            }

            addSinusoidalVoltageSource(name, n_pos, n_neg, offset, amplitude, frequency);
        } catch (const invalid_argument& e) {
            cout << "Error: Invalid number format for sinusoidal source parameters." << endl;
        }
        return;
    }
    if (regex_match(input, match, tranMultiRegex)) {
        try {
            double t_step = stod(match[1].str());
            double t_stop = stod(match[2].str());
            string all_requests_str = match[3].str();

            // Check if the requests part is not for a single V or I analysis to avoid conflict
            regex singleV(R"(V\(\w+\)\s*)");
            regex singleI(R"(I\(\w+\)\s*)");
            if (regex_match(all_requests_str, singleV) || regex_match(all_requests_str, singleI)) {
                // This is a single request, let the old handlers process it.
                // We do nothing here and let the code continue to the next regex checks.
            } else {
                // This is a multi-request command. Process it here.
                stringstream ss(all_requests_str);
                string single_req_str;
                int request_count = 0;

                // Split the request string by spaces and parse each one
                while (ss >> single_req_str) {
                    if (request_count > 0) {
                        cout << "\n----------------------------------------\n" << endl;
                    }

                    regex reqParser(R"(([VI])\((\w+)\))");
                    smatch reqMatch;
                    if (regex_match(single_req_str, reqMatch, reqParser)) {
                        char type = reqMatch[1].str()[0];
                        string name = reqMatch[2].str();

                        // Call the appropriate, existing analysis function
                        if (type == 'V') {
                            performVoltageTransientAnalysis(t_step, t_stop, name);
                        } else if (type == 'I') {
                            performCurrentTransientAnalysis(t_step, t_stop, name);
                        }
                    }
                    request_count++;
                }
                return; // Command handled, exit the function.
            }
        } catch (const exception& e) {
            cout << "Error processing multi-variable transient analysis: " << e.what() << endl;
            return;
        }
    }

    if (regex_match(input, match, tranVoltageRegex)) {
        try {
            double t_step = stod(match[1].str());
            double t_stop = stod(match[2].str());
            string node_name = match[3].str();

            // Call the new analysis function
            performVoltageTransientAnalysis(t_step, t_stop, node_name);

        } catch (const exception& e) {
            cout << "Error: Invalid parameters for transient analysis. " << e.what() << endl;
        }
        return;
    }
    if (regex_match(input, match, tranCurrentRegex)) {
        try {
            double t_step = stod(match[1].str());
            double t_stop = stod(match[2].str());
            string element_name = match[3].str();

            performCurrentTransientAnalysis(t_step, t_stop, element_name);

        } catch (const exception& e) {
            cout << "Error: Invalid parameters for current transient analysis. " << e.what() << endl;
        }
        return;
    }




    // --- Generic syntax error ---
    cout << "Error: Syntax error\n";


}
