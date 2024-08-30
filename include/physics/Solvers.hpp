#pragma once
#include <physics/ODESolver.hpp>
namespace aun{
class EulerSolver : public ODESolver {
    public:
        void solve(RigidBody& body, float dt) override;

};
class RungeKuttaSolver : public ODESolver {
    public:
        void solve(RigidBody& body, float dt) override;
};

void testRungeKuttaSolver();
void testEulerSolver();
glm::vec3 analyticalSolution(float t, float omega, glm::vec3 initial_position, glm::vec3 initial_velocity);

}