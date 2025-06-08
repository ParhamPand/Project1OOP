#include "IdealDiode.h"
#include <iostream>
#include <string>

using namespace std;


Diode::Diode(const string& name, Node* n1, Node* n2)
        : CircuitElement(name, n1, n2, 0.0, ElementType::DIODE) {
    cout << "Info: Ideal Diode '" << getName() <<"' created between node "
         << n1->getName() << " (Anode) and " << n2->getName() << " (Cathode)." << endl;
}

Diode::~Diode() {
    // Destructor
}


void Diode::applyStamps(/* CircuitMatrix& matrix */) const {
    cout << "Applying stamps for Ideal Diode: " << getName() ;
    if (getNode1() && getNode2()) {
        cout << " Connected between Node " << getNode1()->getName()
             << " and Node " << getNode2()->getName() << ".";
    }
    cout << endl;
}