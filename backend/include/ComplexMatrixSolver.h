#ifndef COMPLEXMATRIXSOLVER_H
#define COMPLEXMATRIXSOLVER_H

#include <vector>
#include <complex>
#include <stdexcept>

using Complex = std::complex<double>;

class ComplexMatrixSolver {
private:
    std::vector<std::vector<Complex>> A;
    std::vector<Complex> b;
    int size;

public:
    ComplexMatrixSolver(const std::vector<std::vector<Complex>>& A, const std::vector<Complex>& b);
    std::vector<Complex> solve();
};

#endif // COMPLEXMATRIXSOLVER_H