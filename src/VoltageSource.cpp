#include "VoltageSource.h"
#include <iostream>
#include <string>

using namespace std;

VoltageSource::VoltageSource(const string& name, Node* n1, Node* n2, double voltageValue)
        : CircuitElement(name, n1, n2, voltageValue, ElementType::VOLTAGE_SOURCE) {
    // Voltage can be positive, negative, or zero. No specific warning for non-positive.
    // However, a zero-volt voltage source is effectively a short circuit or a wire,
    // which might be an unusual (but valid) way to define a connection.
    if (voltageValue == 0) {
        cerr << "Info: VoltageSource '" << getName()
             << "' created with zero value: " << voltageValue << " Volts. Acts as a short circuit for MNA." << endl;
    }
}

VoltageSource::~VoltageSource() {
    // Destructor
}

void VoltageSource::applyStamps(/* CircuitMatrix& matrix */) const {
    // Placeholder for MNA stamping logic for a voltage source.
    // A voltage source V between node k (positive) and node m (negative) with value E
    // introduces an auxiliary current variable (e.g., I_V).
    // The equations added to the MNA system are:
    // 1. V_k - V_m = E  (Voltage definition)
    // 2. KCL at node k includes +I_V
    // 3. KCL at node m includes -I_V
    // This usually means expanding the matrix by one row/column for the new current variable.
    cout << "Applying stamps for VoltageSource: " << getName()
         << " Value: " << getValue() << " Volts.";
    if (getNode1() && getNode2()) {
        cout << " Connected between Node " << getNode1()->getId()
             << " (Positive: " << getNode1()->getName() << ")"
             << " and Node " << getNode2()->getId()
             << " (Negative: " << getNode2()->getName() << ").";
    } else if (getNode1()) { // Grounded source, n2 is implicitly ground if null
        cout << " Connected between Node " << getNode1()->getId()
             << " (Positive: " << getNode1()->getName() << ") and ground.";
    } else if (getNode2()) { // Should not happen for Vsrc if n1 is positive
        cout << " Warning: VoltageSource " << getName() << " has null positive node.";
    }
    cout << endl;
}
