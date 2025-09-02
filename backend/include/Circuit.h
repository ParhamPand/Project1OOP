#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <stdexcept>
#include <utility>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>
#include "SinusoidalVoltageSource.h"


// Forward declarations
class Node;
class CircuitElement;
class Resistor;
class Capacitor;
class Inductor;
class VoltageSource;
class CurrentSource;
class SinusoidalVoltageSource;
class Diode;
class ZenerDiode;

class Circuit {
private:
    std::set<std::string> groundedNodes;
    std::map<std::string, std::shared_ptr<Node>> namedNodes;
    std::vector<std::shared_ptr<CircuitElement>> allElements;
    int nextNodeId;
    int mna_extra_vars_count;

    std::shared_ptr<Node> getOrCreateNode(const std::string& nodeName);

    // Analysis functions remain private
    std::vector<std::pair<double, double>> performVoltageTransientAnalysis(double t_start, double t_step, double t_stop, const std::string& output_node_name);
    std::vector<std::pair<double, double>> performCurrentTransientAnalysis(double t_start, double t_step, double t_stop, const std::string& output_element_name);
    void performDCSweepAnalysis(const std::string& sourceName, double start, double stop, double step, const std::vector<std::string>& outputs);

public:
    Circuit();
    ~Circuit();

    // --- Component Addition ---
    std::shared_ptr<Resistor> addResistor(const std::string& name, const std::string& node1Name, const std::string& node2Name, double resistance);
    std::shared_ptr<Capacitor> addCapacitor(const std::string& name, const std::string& node1Name, const std::string& node2Name, double capacitance);
    std::shared_ptr<Inductor> addInductor(const std::string& name, const std::string& node1Name, const std::string& node2Name, double inductance);
    std::shared_ptr<VoltageSource> addVoltageSource(const std::string& name, const std::string& positiveNodeName, const std::string& negativeNodeName, double voltage);
    std::shared_ptr<CurrentSource> addCurrentSource(const std::string& name, const std::string& fromNodeName, const std::string& toNodeName, double current);
    std::shared_ptr<SinusoidalVoltageSource> addSinusoidalVoltageSource(const std::string& name, const std::string& node1Name, const std::string& node2Name, double offset, double amplitude, double frequency, double phase);
    std::vector<std::pair<double, double>> runVoltageTransientAnalysis(double t_start, double t_step, double t_stop, const std::string& output_node_name);
    std::vector<std::pair<double, double>> runCurrentTransientAnalysis(double t_start, double t_step, double t_stop, const std::string& output_element_name);
    std::vector<std::pair<double, double>> runACAnalysis(double omega_start, double omega_stop, int num_steps, const std::string& output_name);
    std::vector<std::pair<double, double>> runPhaseSweepAnalysis(double phase_start, double phase_stop, int num_steps, const std::string& output_name);



    std::shared_ptr<Node> getNode(const std::string& name) const;
    std::shared_ptr<CircuitElement> getElement(const std::string& name) const;
    const std::vector<std::shared_ptr<CircuitElement>>& getElements() const;
    const std::map<std::string, std::shared_ptr<Node>>& getNodes() const;
    const std::set<std::string>& getGroundedNodes() const;
    void mergeNodes(const std::string& nodeToKeep, const std::string& nodeToRemove);
    void setNodeAsGround(const std::string& nodeName);
    void clear();

    // --- Serialization ---
    template<class Archive>
    void serialize(Archive& ar) {
        ar(CEREAL_NVP(groundedNodes), CEREAL_NVP(namedNodes), CEREAL_NVP(allElements), CEREAL_NVP(nextNodeId), CEREAL_NVP(mna_extra_vars_count));
    }
};