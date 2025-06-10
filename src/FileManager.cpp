#include "FileManager.h"
#include "Circuit.h"
#include "CircuitElement.h"
#include "SinusoidalVoltageSource.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

FileManager::FileManager() {}
FileManager::~FileManager() {}

void FileManager::saveCircuit(const Circuit* circuit, const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "' for writing." << std::endl;
        return;
    }

    // ۱. ذخیره تمام المان‌ها
    const auto& elements = circuit->getElements();
    for (const auto* elem : elements) {
        std::string line = "add " + elem->getName() + " " + elem->getNode1()->getName() + " " + elem->getNode2()->getName();

        switch (elem->getElementType()) {
            case ElementType::RESISTOR:
            case ElementType::CAPACITOR:
            case ElementType::INDUCTOR:
            case ElementType::VOLTAGE_SOURCE:
            case ElementType::CURRENT_SOURCE:
                line += " " + std::to_string(elem->getValue());
                break;

            case ElementType::SINUSOIDAL_VOLTAGE_SOURCE: {
                const auto* sinSource = static_cast<const SinusoidalVoltageSource*>(elem);
                line += " SIN(" + std::to_string(sinSource->getOffset()) + " "
                        + std::to_string(sinSource->getAmplitude()) + " "
                        + std::to_string(sinSource->getFrequency()) + ")";
                break;
            }
            case ElementType::DIODE:
            case ElementType::ZENER_DIODE:

                continue;

            default:
                continue;
        }
        outFile << line << std::endl;
    }


    const auto& grounded = circuit->getGroundedNodes();
    for (const auto& nodeName : grounded) {
        outFile << "add GND " << nodeName << std::endl;
    }

    outFile.close();
    std::cout << "Circuit successfully saved to '" << filename << "'." << std::endl;
}

void FileManager::loadCircuit(Circuit* circuit, const std::string& filename) {
    // ۱. مهم‌ترین قدم: پاک کردن کامل مدار فعلی قبل از بارگزاری
    circuit->clear();

    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "' for reading." << std::endl;
        return;
    }

    std::cout << "Loading circuit from '" << filename << "'..." << std::endl;
    std::string line;

    // ۲. خواندن فایل خط به خط و اجرای هر خط به عنوان یک دستور
    while (std::getline(inFile, line)) {
        // از تابع handleCommand خود مدار برای پردازش هر خط استفاده می‌کنیم
        if (!line.empty()) {
            circuit->handleCommand(line);
        }
    }

    inFile.close();
    std::cout << "Circuit successfully loaded." << std::endl;
}

