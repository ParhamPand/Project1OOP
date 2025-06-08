#include "ZenerDiode.h"
#include <iostream>

using namespace std;

ZenerDiode::ZenerDiode(const std::string& name, Node* n1, Node* n2)
        : CircuitElement(name, n1, n2, 0.7, ElementType::ZENER_DIODE) {
    cout << "Info: Zener Diode '" << getName() << "' created between node "
         << n1->getName() << " (Anode) and " << n2->getName() << " (Cathode)." << endl;
}

ZenerDiode::~ZenerDiode() {
    // Destructor
}

void ZenerDiode::applyStamps(/* CircuitMatrix& matrix */) const {
    cout << "Applying stamps for Zener Diode: " << getName();
    if (getNode1() && getNode2()) {
        cout << " Connected between Node " << getNode1()->getName()
             << " and Node " << getNode2()->getName() << ".";
    }
    cout << endl;
}
