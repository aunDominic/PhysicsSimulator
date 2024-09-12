#include <physics/System.hpp>
#include <spdlog/spdlog.h>
namespace aun{
void System::update(float dt){
    for (auto& body : bodies) {
        for (auto force_generator: force_generators){
            force_generator->apply(body);
        }
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
    force_generators.push_back(new Gravity());
    // force_generators.push_back(new AirResistance());
}
System::System(){}
}