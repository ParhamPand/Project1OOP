#include "IdealDiode.h"
#include <iostream>
#include <complex>


Diode::Diode(const std::string& name, std::shared_ptr<Node> n1, std::shared_ptr<Node> n2)
        : CircuitElement(name, n1, n2, 0.0, ElementType::DIODE) {
    std::cout << "Info: Ideal Diode '" << getName() << "' created." << std::endl;
}

Diode::~Diode() {}

void Diode::applyStamps(std::vector<std::vector<double>>& A, std::vector<double>& b, const std::map<std::string, int>& node_map, const std::vector<double>& x_prev, int mna_extra_vars_start_index, double t, double dt) const {

}

void Diode::applyDCStamps(std::vector<std::vector<double>>& A, std::vector<double>& b, const std::map<std::string, int>& node_map, int mna_extra_vars_start_index) const {

}

void Diode::applyACStamps(std::vector<std::vector<Complex>>& A, std::vector<Complex>& b, const std::map<std::string, int>& node_map, int mna_extra_vars_start_index, double omega) const {

}