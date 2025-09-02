#include "CurrentSource.h"
#include <iostream>
#include <vector>
#include <map>
#include <complex>

CurrentSource::CurrentSource(const std::string& name, std::shared_ptr<Node> n1, std::shared_ptr<Node> n2, double currentValue)
        : CircuitElement(name, n1, n2, currentValue, ElementType::CURRENT_SOURCE) {}

CurrentSource::~CurrentSource() {}

void CurrentSource::setValue(double val) {
    this->value = val;
}

void CurrentSource::applyStamps(std::vector<std::vector<double>>& A,
                                std::vector<double>& b,
                                const std::map<std::string, int>& node_map,
                                const std::vector<double>& x_prev,
                                int mna_extra_vars_start_index,
                                double t, double dt) const {
    int n1_idx = (node_map.count(node1->getName())) ? node_map.at(node1->getName()) : -1;
    int n2_idx = (node_map.count(node2->getName())) ? node_map.at(node2->getName()) : -1;

    if (n1_idx != -1) b[n1_idx] -= getValue();
    if (n2_idx != -1) b[n2_idx] += getValue();
}

void CurrentSource::applyDCStamps(std::vector<std::vector<double>>& A,
                                  std::vector<double>& b,
                                  const std::map<std::string, int>& node_map,
                                  int mna_extra_vars_start_index) const {
    int n1_idx = (node_map.count(node1->getName())) ? node_map.at(node1->getName()) : -1;
    int n2_idx = (node_map.count(node2->getName())) ? node_map.at(node2->getName()) : -1;

    if (n1_idx != -1) b[n1_idx] -= getValue();
    if (n2_idx != -1) b[n2_idx] += getValue();
}

void CurrentSource::applyACStamps(std::vector<std::vector<Complex>>& A, std::vector<Complex>& b, const std::map<std::string, int>& node_map, int mna_extra_vars_start_index, double omega) const {}