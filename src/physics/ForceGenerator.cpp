#include <physics/ForceGenerator.hpp>

namespace aun {

void AirResistance::apply(RigidBody* body, float dt){
    auto magnitude = 0.5f * (body->velocity * body->velocity) * fluidDensity * coefficient * body->geometry->getCrossSectionArea();
    body->applyForce(-glm::normalize(body->velocity) * magnitude); // Apply the force opposite to the velocity's direction
}
void Gravity::apply(RigidBody* body, float dt){
    spdlog::debug("Applying gravitational force...");
    if (body->inverseMass == 0) return;
    body->velocity += glm::vec3(0,-1,0) * coefficient * dt;
}
std::string Gravity::toString() const {
    return "Gravity";
}

std::string AirResistance::toString() const{
    return "Air Resistance";
}
}