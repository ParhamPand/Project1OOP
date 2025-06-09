#include "ZenerDiode.h"
#include <iostream>
#include <vector>
#include <map>

// سازنده اصلی شما، بدون تغییر
ZenerDiode::ZenerDiode(const std::string& name, Node* n1, Node* n2)
        : CircuitElement(name, n1, n2, 0.7, ElementType::ZENER_DIODE) {
    std::cout << "Info: Zener Diode '" << getName() << "' created between node "
              << n1->getName() << " (Anode) and " << n2->getName() << " (Cathode)." << std::endl;
}

// مخرب اصلی شما، بدون تغییر
ZenerDiode::~ZenerDiode() {}

// پیاده‌سازی applyStamps با بدنه خالی برای رفع خطای کامپایل
void ZenerDiode::applyStamps(std::vector<std::vector<double>>& A,
                             std::vector<double>& b,
                             const std::map<std::string, int>& node_map,
                             const std::vector<double>& x_prev,
                             int mna_extra_vars_start_index,
                             double t, double dt) const {
    // این تابع فعلاً هیچ کاری انجام نمی‌دهد.
    // دیود زنر در تحلیل گذرا نادیده گرفته می‌شود.
}