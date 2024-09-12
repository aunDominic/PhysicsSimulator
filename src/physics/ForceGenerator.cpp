#include <physics/ForceGenerator.hpp>

namespace aun {

void AirResistance::apply(RigidBody* body){
    auto magnitude = 0.5f * (body->velocity * body->velocity) * fluidDensity * coefficient * body->geometry->getCrossSectionArea();
    body->applyForce(-glm::normalize(body->velocity) * magnitude); // Apply the force opposite to the velocity's direction
}
void Gravity::apply(RigidBody* body){
    spdlog::debug("Applying gravitational force...");
    body->applyForce(glm::vec3(0,-1,0) * coefficient * body->mass);
}
}