#include "SinusoidalVoltageSource.h"
#include "Node.h"
#include <iostream>

SinusoidalVoltageSource::SinusoidalVoltageSource(const std::string& name, Node* n1, Node* n2, double offset, double amplitude, double frequency)
// مقدار پایه را می‌توانیم صفر در نظر بگیریم چون پارامترهای خودمان را داریم
        : CircuitElement(name, n1, n2, 0.0, ElementType::SINUSOIDAL_VOLTAGE_SOURCE),
          offset(offset), amplitude(amplitude), frequency(frequency) {

}

SinusoidalVoltageSource::~SinusoidalVoltageSource() {}

void SinusoidalVoltageSource::applyStamps(/* CircuitMatrix& matrix */) const {
    // Placeholder for MNA stamping logic
    std::cout << "Applying stamps for SinusoidalVoltageSource: " << getName() << std::endl;
}

void SinusoidalVoltageSource::printDetails() const {
    std::cout << "Element: " << getName()
              << ", Type: " << getElementTypeString()
              << ", Voffset: " << offset << "V"
              << ", Vamplitude: " << amplitude << "V"
              << ", Frequency: " << frequency << "Hz";
    if (getNode1() && getNode2()) {
        std::cout << ", Nodes: (Positive: " << getNode1()->getName()
                  << ", Negative: " << getNode2()->getName() << ")";
    }
    std::cout << std::endl;
}