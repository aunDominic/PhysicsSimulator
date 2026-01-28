#include <physics/Solvers.hpp>

namespace aun{
// Simple harmonic oscillator analytical solution
glm::vec3 analyticalSolution(float t, float omega, glm::vec3 initial_position, glm::vec3 initial_velocity) {
    return initial_position * std::cos(omega * t) + (initial_velocity / omega) * std::sin(omega * t);
}
}