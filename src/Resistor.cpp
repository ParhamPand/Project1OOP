#include "Resistor.h"
#include <iostream> // For cerr, cout, endl
#include <string>   // For string, to_string

// Using directives for std namespace
using namespace std;

Resistor::Resistor(const string& name, Node* n1, Node* n2, double resistanceValue)
        : CircuitElement(name, n1, n2, resistanceValue, ElementType::RESISTOR) {
    if (resistanceValue <= 0) {
        cerr << "Warning: Resistor '" << getName()
             << "' created with non-positive value: " << resistanceValue << " Ohms." << endl;
    }
}

Resistor::~Resistor() {
}

void Resistor::applyStamps(/* CircuitMatrix& matrix */) const {
    // Placeholder for MNA stamping logic
    cout << "Applying stamps for Resistor: " << getName()
         << " Value: " << getValue() << " Ohms.";
    if (getNode1() && getNode2()) {
        cout << " Connected between Node " << getNode1()->getId()
             << " (" << getNode1()->getName() << ")"
             << " and Node " << getNode2()->getId()
             << " (" << getNode2()->getName() << ").";
    }
    cout << endl;
}