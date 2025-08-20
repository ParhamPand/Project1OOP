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
#include <cereal/types/polymorphic.hpp>

CEREAL_REGISTER_TYPE(Resistor);
CEREAL_REGISTER_TYPE(Capacitor);
CEREAL_REGISTER_TYPE(Inductor);
CEREAL_REGISTER_TYPE(VoltageSource);
CEREAL_REGISTER_TYPE(CurrentSource);
CEREAL_REGISTER_TYPE(Diode);
CEREAL_REGISTER_TYPE(ZenerDiode);
CEREAL_REGISTER_TYPE(SinusoidalVoltageSource);

CEREAL_REGISTER_POLYMORPHIC_RELATION(CircuitElement, Resistor)
CEREAL_REGISTER_POLYMORPHIC_RELATION(CircuitElement, Capacitor)
CEREAL_REGISTER_POLYMORPHIC_RELATION(CircuitElement, Inductor)
CEREAL_REGISTER_POLYMORPHIC_RELATION(CircuitElement, VoltageSource)
CEREAL_REGISTER_POLYMORPHIC_RELATION(CircuitElement, CurrentSource)
CEREAL_REGISTER_POLYMORPHIC_RELATION(CircuitElement, Diode)
CEREAL_REGISTER_POLYMORPHIC_RELATION(CircuitElement, ZenerDiode)
CEREAL_REGISTER_POLYMORPHIC_RELATION(CircuitElement, SinusoidalVoltageSource)

using namespace std;

Circuit::Circuit() : nextNodeId(0), mna_extra_vars_count(0) {}

Circuit::~Circuit() {
    clear();
}

void Circuit::clear() {
    allElements.clear();
    namedNodes.clear();
    groundedNodes.clear();
    nextNodeId = 0;
    mna_extra_vars_count = 0;
    cout << "Circuit cleared." << endl;
}

shared_ptr<Node> Circuit::getOrCreateNode(const string& nodeName) {
    auto it = namedNodes.find(nodeName);
    if (it == namedNodes.end()) {
        auto newNode = make_shared<Node>(nextNodeId++, nodeName);
        namedNodes[nodeName] = newNode;
        return newNode;
    }
    return it->second;
}

shared_ptr<CircuitElement> Circuit::getElement(const string& name) const {
    for (const auto& elem : allElements) {
        if (elem->getName() == name) {
            return elem;
        }
    }
    return nullptr;
}



shared_ptr<Resistor> Circuit::addResistor(const string& name, const string& node1Name, const string& node2Name, double resistance) {
    auto n1 = getOrCreateNode(node1Name);
    auto n2 = getOrCreateNode(node2Name);
    auto res = make_shared<Resistor>(name, n1, n2, resistance);
    allElements.push_back(res);
    return res;
}

shared_ptr<Capacitor> Circuit::addCapacitor(const string& name, const string& node1Name, const string& node2Name, double capacitance) {
    auto n1 = getOrCreateNode(node1Name);
    auto n2 = getOrCreateNode(node2Name);
    auto cap = make_shared<Capacitor>(name, n1, n2, capacitance);
    allElements.push_back(cap);
    return cap;
}

shared_ptr<Inductor> Circuit::addInductor(const string& name, const string& node1Name, const string& node2Name, double inductance) {
    auto n1 = getOrCreateNode(node1Name);
    auto n2 = getOrCreateNode(node2Name);
    auto ind = make_shared<Inductor>(name, n1, n2, inductance);
    ind->mna_branch_index = mna_extra_vars_count++;
    allElements.push_back(ind);
    return ind;
}

shared_ptr<VoltageSource> Circuit::addVoltageSource(const string& name, const string& positiveNodeName, const string& negativeNodeName, double voltage) {
    auto n_pos = getOrCreateNode(positiveNodeName);
    auto n_neg = getOrCreateNode(negativeNodeName);
    auto v_src = make_shared<VoltageSource>(name, n_pos, n_neg, voltage);
    v_src->mna_branch_index = mna_extra_vars_count++;
    allElements.push_back(v_src);
    return v_src;
}

shared_ptr<CurrentSource> Circuit::addCurrentSource(const string& name, const string& fromNodeName, const string& toNodeName, double current) {
    auto n_from = getOrCreateNode(fromNodeName);
    auto n_to = getOrCreateNode(toNodeName);
    auto i_src = make_shared<CurrentSource>(name, n_from, n_to, current);
    allElements.push_back(i_src);
    return i_src;
}

std::shared_ptr<SinusoidalVoltageSource> Circuit::addSinusoidalVoltageSource(const std::string& name, const std::string& node1Name, const std::string& node2Name, double offset, double amplitude, double frequency, double phase) {
    auto n1 = getOrCreateNode(node1Name);
    auto n2 = getOrCreateNode(node2Name);
    auto src = std::make_shared<SinusoidalVoltageSource>(name, n1, n2, offset, amplitude, frequency, phase);
    src->mna_branch_index = mna_extra_vars_count++;
    allElements.push_back(src);
    return src;
}



std::shared_ptr<Node> Circuit::getNode(const string& name) const {
    auto it = namedNodes.find(name);
    if (it != namedNodes.end()) {
        return it->second;
    }
    return nullptr;
}

// Return a CONST reference to the vector
const vector<shared_ptr<CircuitElement>>& Circuit::getElements() const {
    return allElements;
}

// Return a CONST reference to the map
const map<string, shared_ptr<Node>>& Circuit::getNodes() const {
    return namedNodes;
}

// Return a CONST reference to the set
const std::set<std::string>& Circuit::getGroundedNodes() const {
    return this->groundedNodes;
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
        for (const auto& elem : allElements) {
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


    if (groundedNodes.empty()) {
        cout << "Error: No ground node defined. Analysis is not possible." << endl;
        return;
    }

    auto target_element = getElement(output_element_name);
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
    for (const auto& elem : allElements) {
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

        for (const auto& elem : allElements) {
            elem->applyStamps(A, b, node_map, x_prev, node_count, t, t_step);
        }

        try {
            MatrixSolver solver(A, b);
            vector<double> x_current = solver.solve();

            double output_current = 0.0;
            // --- Current Calculation Logic ---
            auto n1 = target_element->getNode1();
            auto n2 = target_element->getNode2();
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

void Circuit::performDCSweepAnalysis(const std::string& sourceName, double start, double stop, double step, const std::vector<std::string>& outputs) {

    auto sweepSource = getElement(sourceName);
    if (!sweepSource) {
        std::cerr << "Error: Source '" << sourceName << "' for DC sweep not found." << std::endl;
        return;
    }


    std::map<std::string, int> node_map;
    int node_count = 0;
    for (const auto& pair : namedNodes) {
        if (groundedNodes.find(pair.first) == groundedNodes.end()) {
            node_map[pair.first] = node_count++;
        }
    }
    int system_size = node_count + mna_extra_vars_count;


    std::cout << sourceName;
    for (const auto& out : outputs) {
        std::cout << "\t" << out;
    }
    std::cout << std::endl;


    for (double currentVal = start; (step > 0) ? (currentVal <= stop + 1e-9) : (currentVal >= stop - 1e-9); currentVal += step) {
        sweepSource->setValue(currentVal);


        std::vector<std::vector<double>> A(system_size, std::vector<double>(system_size, 0.0));
        std::vector<double> b(system_size, 0.0);

        for (const auto& elem : allElements) {
            elem->applyDCStamps(A, b, node_map, node_count);
        }

        std::vector<double> x_current;
        try {
            MatrixSolver solver(A, b);
            x_current = solver.solve();
        } catch (const std::runtime_error& e) {
            std::cerr << "Error during DC sweep at " << sourceName << " = " << currentVal << ": " << e.what() << std::endl;
            return;
        }


        std::cout << currentVal;
        std::regex out_parser(R"(([VI])\((\w+)\))");
        std::smatch out_match;

        for (const auto& req : outputs) {
            double out_val = 0.0;
            if (std::regex_match(req, out_match, out_parser)) {
                std::string type = out_match[1];
                std::string name = out_match[2];

                if (type == "V") {
                    if (node_map.count(name)) {
                        out_val = x_current[node_map.at(name)];
                    } else if (groundedNodes.count(name) || name == "0") {
                        out_val = 0.0;
                    }
                } else if (type == "I") {
                    auto elem = getElement(name);
                    if (elem) {
                        if (elem->getElementType() == ElementType::VOLTAGE_SOURCE || elem->getElementType() == ElementType::INDUCTOR) {
                            out_val = x_current[node_count + elem->mna_branch_index];
                        } else if (elem->getElementType() == ElementType::RESISTOR) {
                            auto n1 = elem->getNode1();
                            auto n2 = elem->getNode2();
                            double v1 = (n1 && node_map.count(n1->getName())) ? x_current[node_map.at(n1->getName())] : 0.0;
                            double v2 = (n2 && node_map.count(n2->getName())) ? x_current[node_map.at(n2->getName())] : 0.0;
                            out_val = (elem->getValue() > 1e-9) ? (v1 - v2) / elem->getValue() : 0.0;
                        } else if (elem->getElementType() == ElementType::CURRENT_SOURCE) {
                            out_val = elem->getValue();
                        }
                    }
                }
            }
            std::cout << "\t" << out_val;
        }
        std::cout << std::endl;
    }
}
