#include <physics/System.hpp>
#include <spdlog/spdlog.h>
namespace aun{
void System::update(float dt){
    for (auto& body : bodies) {
        spdlog::debug("Force generators size: {}", force_generators.size());
        for (auto force_generator: force_generators){
            spdlog::debug("Applying force generator {}", force_generator->toString());
            force_generator->apply(body, dt);
        }
    }
    // Then, resolve collisions
    float restitution = 1.0f; // 1.0f means perfectly elastic
    float velocityThreshold = 0.005f;
    float restingThreshold = 0.35f; // New threshold for resting contacts
    float angularVelocityThreshold = 0.005f;
    float correctionFactor = 0.5f; // tweak as needed
    float penetrationSlack = 0.005f;

    for (int i = 0; i < bodies.size(); i++){
        for (int j = i + 1; j < bodies.size(); j++){
            CollisionInfo info = collisionDetector->checkCollision(bodies[i]->geometry, bodies[j]->geometry);
            if (info.hasCollision){ // Only resolve if the bodies are moving towards each other
                spdlog::debug("Velocity before impulse: i({}) j({})", vec3_to_string(bodies[i]->velocity, 4), vec3_to_string(bodies[j]->velocity, 4));
                glm::vec3 supportA = bodies[i]->geometry->getSupport(info.normal);
                glm::vec3 supportB = bodies[j]->geometry->getSupport(-info.normal);

                // Contact point
                glm::vec3 contactPoint = (supportA + supportB) * 0.5f;

                // Relative velocity at contact point
                glm::vec3 rA = contactPoint - bodies[i]->position;
                glm::vec3 rB = contactPoint - bodies[j]->position;

                glm::vec3 vA = bodies[i]->velocity + glm::cross(bodies[i]->angularVelocity, rA);
                glm::vec3 vB = bodies[j]->velocity + glm::cross(bodies[j]->angularVelocity, rB);

                glm::vec3 relativeVelocity = vA - vB;
                float normalVelocity = glm::dot(relativeVelocity, info.normal);
                // Coefficient of restitution needs to be zero for a system to reach equilbrium.
                float scaledRestitution = (std::abs(normalVelocity) < restingThreshold) ? 0.0f : restitution;

                float numerator = -(1.0f + scaledRestitution) * normalVelocity;

                glm::mat3 invInertiaA = (bodies[i]->inverseMass == 0) 
                    ? glm::mat3(0.0f) 
                    : glm::inverse(bodies[i]->geometry->getInertiaTensor(bodies[i]->mass));
                glm::mat3 invInertiaB = (bodies[j]->inverseMass == 0) 
                    ? glm::mat3(0.0f) 
                    : glm::inverse(bodies[j]->geometry->getInertiaTensor(bodies[j]->mass));                log_mat3(invInertiaA);
                float linearTerm = bodies[i]->inverseMass + bodies[j]->inverseMass;
                glm::vec3 angularTermA = glm::cross(invInertiaA * glm::cross(rA, info.normal), rA);
                glm::vec3 angularTermB = glm::cross(invInertiaB * glm::cross(rB, info.normal), rB);
                float angularTerm = glm::dot(info.normal, angularTermA + angularTermB);
                float denominator = linearTerm + angularTerm;
                spdlog::debug("Angular A: {}, Angular B: {}, normal: {}, rA: {}, rb: {}", vec3_to_string(angularTermA), vec3_to_string(angularTermB), vec3_to_string(info.normal), vec3_to_string(rA), vec3_to_string(rB));  
                spdlog::debug("Linear term: {}, Angular term: {}, restitution: {}", linearTerm, angularTerm, scaledRestitution);

                glm::vec3 impulse = (numerator / denominator) * info.normal;
                spdlog::debug("Impulse: {} between {}, {}, denom {}", vec3_to_string(impulse), i, j, denominator);

                // Apply impulse to linear and angular velocities
                bodies[i]->velocity += impulse * bodies[i]->inverseMass;
                bodies[j]->velocity -= impulse * bodies[j]->inverseMass;

                // Clamp small velocities
                if (glm::length(bodies[i]->velocity) < velocityThreshold) {
                    bodies[i]->velocity = glm::vec3(0.0f);
                }
                if (glm::length(bodies[j]->velocity) < velocityThreshold) {
                    bodies[j]->velocity = glm::vec3(0.0f);
                }


                spdlog::debug("Velocity after impulse: i({}) j({})", vec3_to_string(bodies[i]->velocity, 4), vec3_to_string(bodies[j]->velocity));

                bodies[i]->angularVelocity += invInertiaA * glm::cross(rA, impulse);
                bodies[j]->angularVelocity -= invInertiaB * glm::cross(rB, impulse);
                              // Clamp small velocities
                if (glm::length(bodies[i]->angularVelocity) < angularVelocityThreshold) {
                    bodies[i]->angularVelocity = glm::vec3(0.0f);
                }
                if (glm::length(bodies[j]->angularVelocity) < angularVelocityThreshold) {
                    bodies[j]->angularVelocity = glm::vec3(0.0f);
                }

                // Positional correction to remove penetration
                float penetrationDepth = glm::max(info.penetrationDepth - penetrationSlack, 0.0f);
                glm::vec3 correction = (penetrationDepth * correctionFactor) * info.normal;
                bodies[i]->position -= correction * bodies[i]->inverseMass;
                bodies[j]->position += correction * bodies[j]->inverseMass;
                
            }
        }
    }
    // Update state
    // Applyin forces.
    for (auto& body : bodies) {
        // spdlog::debug("Force generators size: {}", force_generators.size());
        // for (auto force_generator: force_generators){
        //     spdlog::debug("Applying force generator {}", force_generator->toString());
        //     force_generator->apply(body, dt);
        // }
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
    spdlog::debug("Adding Gravity.");
    force_generators.push_back(new Gravity());
    // force_generators.push_back(new AirResistance());
}
System::System(){}
}