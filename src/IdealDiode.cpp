#include "IdealDiode.h"
#include <iostream>
#include <string>

using namespace std;

// پیاده‌سازی سازنده ویرایش‌شده
Diode::Diode(const string& name, Node* n1, Node* n2, const string& model)
        : CircuitElement(name, n1, n2, 0.0, ElementType::DIODE), model(model) {
    cout << "Info: Diode '" << getName() << "' with model '" << model << "' created between node "
         << n1->getName() << " (Anode) and " << n2->getName() << " (Cathode)." << endl;
}

Diode::~Diode() {
    // Destructor
}

// پیاده‌سازی تابع برای گرفتن مدل
string Diode::getModel() const {
    return model;
}

void Diode::applyStamps(/* CircuitMatrix& matrix */) const {
    cout << "Applying stamps for Diode: " << getName() << ", Model: " << getModel();
    if (getNode1() && getNode2()) {
        cout << " Connected between Node " << getNode1()->getName()
             << " and Node " << getNode2()->getName() << ".";
    }
    cout << endl;
}