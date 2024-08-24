#pragma once
#include <physics/ODESolver.hpp>
namespace aun{
class EulerSolver : public ODESolver {
    void solve(RigidBody& body, float dt) override;
};
class RungeKuttaSolver : public ODESolver {
    void solve(RigidBody& body, float dt) override;
};
}