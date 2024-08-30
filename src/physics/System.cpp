#include <physics/System.hpp>
#include <spdlog/spdlog.h>
namespace aun{
void System::update(float dt){
            for (auto& body : bodies) {
                solver->solve(*body, dt);
            }
        }
void System::addRigidBody(RigidBody* body){
    spdlog::debug("Adding rigid body...");
    spdlog::debug("Number of rigid bodies:{}", bodies.size());
    bodies.push_back(std::move(body));
    spdlog::debug("Number of rigid bodies:{}", bodies.size());
}
const std::vector<RigidBody*>& System::getBodies() const {
    return bodies;
}
System::System(float dt, ODESolver *solver): dt(dt), solver(std::move(solver)){
}
System::System(){}
}