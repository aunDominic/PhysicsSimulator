#pragma once 

#include <vector>
#include <physics/ODESolver.hpp>
namespace aun{
    class System{
    private:
        std::vector<std::unique_ptr<RigidBody>> bodies;
        std::unique_ptr<ODESolver> solver;
        float dt;
    public: 
        System();
        System(float dt, std::unique_ptr<ODESolver> solver);
        void update(float dt);
        void addRigidBody(std::unique_ptr<RigidBody> body);
        const std::vector<std::unique_ptr<RigidBody>>& getBodies() const;
    };
}