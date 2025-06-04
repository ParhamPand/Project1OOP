#include "Capacitor.h"
#include <iostream>
#include <string>


using namespace std;

Capacitor::Capacitor(const string& name, Node* n1, Node* n2, double capacitanceValue)
        : CircuitElement(name, n1, n2, capacitanceValue, ElementType::CAPACITOR) {
    if (capacitanceValue <= 0) {
        cerr << "Warning: Capacitor '" << getName()
             << "' created with non-positive value: " << capacitanceValue << " Farads." << endl;
    }
}

Capacitor::~Capacitor() {
}

void Capacitor::applyStamps(/* CircuitMatrix& matrix */) const {
    // Placeholder for MNA stamping logic for a capacitor
    // This will be different from a resistor, especially in AC/transient analysis
    cout << "Applying stamps for Capacitor: " << getName()
         << " Value: " << getValue() << " Farads.";
    if (getNode1() && getNode2()) {
        cout << " Connected between Node " << getNode1()->getId()
             << " (" << getNode1()->getName() << ")"
             << " and Node " << getNode2()->getId()
             << " (" << getNode2()->getName() << ").";
    }
    cout << endl;
    // Example for DC analysis (open circuit):
    // For DC, a capacitor is an open circuit, so it might not add conductance terms
    // For transient analysis, it would be G = C/dt or similar
}