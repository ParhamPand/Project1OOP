#include "IdealDiode.h"
#include <iostream>
#include <vector>
#include <map>

// سازنده اصلی شما، بدون تغییر
Diode::Diode(const std::string& name, Node* n1, Node* n2)
        : CircuitElement(name, n1, n2, 0.0, ElementType::DIODE) {
    std::cout << "Info: Ideal Diode '" << getName() << "' created between node "
              << n1->getName() << " (Anode) and " << n2->getName() << " (Cathode)." << std::endl;
}

// مخرب اصلی شما، بدون تغییر
Diode::~Diode() {}

// پیاده‌سازی applyStamps با بدنه خالی برای رفع خطای کامپایل
void Diode::applyStamps(std::vector<std::vector<double>>& A,
                        std::vector<double>& b,
                        const std::map<std::string, int>& node_map,
                        const std::vector<double>& x_prev,
                        int mna_extra_vars_start_index,
                        double t, double dt) const {
    // این تابع فعلاً هیچ کاری انجام نمی‌دهد.
    // دیود در تحلیل گذرا نادیده گرفته می‌شود.
}