#include "MatrixSolver.h"
#include <cmath>
#include <algorithm>
#include <stdexcept>

MatrixSolver::MatrixSolver(const std::vector<std::vector<double>>& A, const std::vector<double>& b)
        : A(A), b(b) {
    this->size = b.size();
    if (size == 0 || A.size() != size || A[0].size() != size) {
        throw std::invalid_argument("Invalid matrix or vector dimensions for MatrixSolver.");
    }
}

std::vector<double> MatrixSolver::solve() {
    // چون سازنده یک کپی از ماتریس‌ها را دریافت کرده، دیگر نیازی به کپی مجدد نیست.

    // --- Forward Elimination with Partial Pivoting ---
    for (int i = 0; i < size; ++i) {
        int max_row = i;
        for (int k = i + 1; k < size; ++k) {
            if (std::abs(A[k][i]) > std::abs(A[max_row][i])) {
                max_row = k;
            }
        }
        std::swap(A[i], A[max_row]);
        std::swap(b[i], b[max_row]);

        if (std::abs(A[i][i]) < 1e-12) {
            throw std::runtime_error("Matrix is singular or numerically unstable.");
        }

        for (int k = i + 1; k < size; ++k) {
            double factor = A[k][i] / A[i][i];
            for (int j = i; j < size; ++j) {
                A[k][j] -= factor * A[i][j];
            }
            b[k] -= factor * b[i];
        }
    }

    // --- Back Substitution ---
    std::vector<double> x(size);
    for (int i = size - 1; i >= 0; --i) {
        if (std::abs(A[i][i]) < 1e-12) {
            throw std::runtime_error("Matrix is singular or numerically unstable.");
        }
        double sum = 0;
        for (int j = i + 1; j < size; ++j) {
            sum += A[i][j] * x[j];
        }
        x[i] = (b[i] - sum) / A[i][i];
    }

    return x;
}