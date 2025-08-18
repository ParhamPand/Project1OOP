#pragma once

#include <vector>
#include <stdexcept>

/**
 * @class MatrixSolver
 * @brief یک کلاس مستقل برای حل دستگاه معادلات خطی Ax = b.
 * این کلاس ماتریس‌ها را به عنوان ورودی دریافت کرده و حل می‌کند.
 */
class MatrixSolver {
private:
    std::vector<std::vector<double>> A;
    std::vector<double> b;
    int size;

public:
    /**
     * @brief سازنده‌ای که ماتریس‌ها را برای حل دریافت می‌کند.
     * @param A ماتریس ضرایب.
     * @param b بردار سمت راست.
     */
    MatrixSolver(const std::vector<std::vector<double>>& A, const std::vector<double>& b);

    /**
     * @brief دستگاه معادلات را حل می‌کند.
     * @return بردار جواب x.
     */
    std::vector<double> solve();
};