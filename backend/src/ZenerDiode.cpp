#include "ZenerDiode.h"
#include <iostream>

ZenerDiode::ZenerDiode(const std::string& name, std::shared_ptr<Node> n1, std::shared_ptr<Node> n2)
        : CircuitElement(name, n1, n2, 0.7, ElementType::ZENER_DIODE) { // Assuming 0.7V is a default value
    std::cout << "Info: Zener Diode '" << getName() << "' created." << std::endl;
}

ZenerDiode::~ZenerDiode() {}

void ZenerDiode::applyStamps(std::vector<std::vector<double>>& A, std::vector<double>& b, const std::map<std::string, int>& node_map, const std::vector<double>& x_prev, int mna_extra_vars_start_index, double t, double dt) const {
    // Logic for transient analysis
}

void ZenerDiode::applyDCStamps(std::vector<std::vector<double>>& A, std::vector<double>& b, const std::map<std::string, int>& node_map, int mna_extra_vars_start_index) const {
    // Logic for DC analysis
}