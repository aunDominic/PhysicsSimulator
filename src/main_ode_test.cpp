#include <physics/Solvers.hpp>
#include <iostream>
int main() {
    std::cout<< "EULER:\n";
    aun::testEulerSolver();
    std::cout << "\n\n\n RUNGE KUTTA: \n";
    aun::testRungeKuttaSolver();
    return 0;
}