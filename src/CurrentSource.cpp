#include "CurrentSource.h"
#include <iostream>
#include <string>

using namespace std;

CurrentSource::CurrentSource(const string& name, Node* n1, Node* n2, double currentValue)
        : CircuitElement(name, n1, n2, currentValue, ElementType::CURRENT_SOURCE) {
    // Current can be positive or negative (indicating direction).
}

CurrentSource::~CurrentSource() {
    // Destructor
}

void CurrentSource::applyStamps(/* CircuitMatrix& matrix */) const {
    // Placeholder for MNA stamping logic for a current source.
    // A current source I flowing from node k to node m with value J:
    // - Adds -J to the RHS vector at the row corresponding to node k.
    // - Adds +J to the RHS vector at the row corresponding to node m.
    // If one node is ground, only one entry is made to the RHS.
    cout << "Applying stamps for CurrentSource: " << getName()
         << " Value: " << getValue() << " Amps.";
    if (getNode1() && getNode2()) {
        cout << " Current flows from Node " << getNode1()->getId()
             << " (" << getNode1()->getName() << ")"
             << " to Node " << getNode2()->getId()
             << " (" << getNode2()->getName() << ") through external circuit.";
    }
    cout << endl;
}