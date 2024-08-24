#pragma once

#include "RigidBody.hpp"
namespace aun{
    class ODESolver {
        public:
            virtual void solve(RigidBody& body, float dt) = 0;
            virtual ~ODESolver() = default;
    };
}
