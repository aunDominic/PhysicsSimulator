#include <physics/System.hpp>
namespace aun{
void System::update(float dt){
            for (auto& body : bodies) {
                solver->solve(*body, dt);
            }
        }
void System::addRigidBody(std::unique_ptr<RigidBody> body){
            bodies.push_back(std::move(body));
        }
const std::vector<std::unique_ptr<RigidBody>>& System::getBodies() const {
        return bodies;
    }
System::System(float dt, std::unique_ptr<ODESolver> solver): dt(dt), solver(std::move(solver)){
}
System::System(){}
}