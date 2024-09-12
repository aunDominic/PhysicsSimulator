#pragma once 

#include <vector>
#include <physics/ODESolver.hpp>
#include <physics/ForceGenerator.hpp>
namespace aun{
    class System{
    private:
        std::vector<RigidBody*> bodies;
        std::vector<ForceGenerator*> force_generators;
        ODESolver *solver;
        float dt;
    public: 
        System();
        System(float dt, ODESolver *solver);
        void update(float dt);
        void addRigidBody(RigidBody *body);
        const std::vector<RigidBody*>& getBodies() const;
    };
}