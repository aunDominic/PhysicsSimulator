#include <physics/System.hpp>
#include <spdlog/spdlog.h>
namespace aun{
void System::resolveCollision(RigidBody *a, RigidBody *b){
    float restitution = 1.0f; // 1.0f means perfectly elastic
    float velocityThreshold = 0.005f;
    float restingThreshold = 0.35f; // New threshold for resting contacts
    float angularVelocityThreshold = 0.005f;
    float correctionFactor = 0.5f; // tweak as needed
    float penetrationSlack = 0.005f;
    
    CollisionInfo info = collisionDetector->checkCollision(a->geometry, b->geometry);
    if (info.hasCollision){ // Only resolve if the bodies are moving towards each other
        spdlog::debug("Velocity before impulse: i({}) j({})", vec3_to_string(a->velocity, 4), vec3_to_string(b->velocity, 4));
        glm::vec3 supportA = a->geometry->getSupport(info.normal);
        glm::vec3 supportB = b->geometry->getSupport(-info.normal);

        // Contact point
        glm::vec3 contactPoint = supportA;

        // Relative velocity at contact point
        glm::vec3 rA = contactPoint - a->position;
        glm::vec3 rB = contactPoint - b->position;

        glm::vec3 vA = a->velocity + glm::cross(a->angularVelocity, rA);
        glm::vec3 vB = b->velocity + glm::cross(b->angularVelocity, rB);

        glm::vec3 relativeVelocity = vA - vB;
        float normalVelocity = glm::dot(relativeVelocity, info.normal);
        // Coefficient of restitution needs to be zero for a system to reach equilbrium.
        float scaledRestitution = (std::abs(normalVelocity) < restingThreshold) ? 0.0f : restitution;

        float numerator = -(1.0f + scaledRestitution) * normalVelocity;

        glm::mat3 invInertiaA = (a->inverseMass == 0) 
            ? glm::mat3(0.0f) 
            : glm::inverse(a->geometry->getInertiaTensor(a->mass));
        glm::mat3 invInertiaB = (b->inverseMass == 0) 
            ? glm::mat3(0.0f) 
            : glm::inverse(b->geometry->getInertiaTensor(b->mass));                log_mat3(invInertiaA);
        float linearTerm = a->inverseMass + b->inverseMass;
        glm::vec3 angularTermA = glm::cross(invInertiaA * glm::cross(rA, info.normal), rA);
        glm::vec3 angularTermB = glm::cross(invInertiaB * glm::cross(rB, info.normal), rB);
        float angularTerm = glm::dot(info.normal, angularTermA + angularTermB);
        float denominator = linearTerm + angularTerm;
        spdlog::debug("Angular A: {}, Angular B: {}, normal: {}, rA: {}, rb: {}", vec3_to_string(angularTermA), vec3_to_string(angularTermB), vec3_to_string(info.normal), vec3_to_string(rA), vec3_to_string(rB));  
        spdlog::debug("Linear term: {}, Angular term: {}, restitution: {}", linearTerm, angularTerm, scaledRestitution);

        glm::vec3 impulse = (numerator / denominator) * info.normal;
        spdlog::debug("Impulse: {} between {}, {}, denom {}", vec3_to_string(impulse), 0, 1, denominator);

        // Apply impulse to linear and angular velocities
        a->velocity += impulse * a->inverseMass;
        b->velocity -= impulse * b->inverseMass;

        // Clamp small velocities
        if (glm::length(a->velocity) < velocityThreshold) {
            a->velocity = glm::vec3(0.0f);
        }
        if (glm::length(b->velocity) < velocityThreshold) {
            b->velocity = glm::vec3(0.0f);
        }


        spdlog::debug("Velocity after impulse: i({}) j({})", vec3_to_string(a->velocity, 4), vec3_to_string(b->velocity));

        a->angularVelocity += invInertiaA * glm::cross(rA, impulse);
        b->angularVelocity -= invInertiaB * glm::cross(rB, impulse);
                        // Clamp small velocities
        if (glm::length(a->angularVelocity) < angularVelocityThreshold) {
            a->angularVelocity = glm::vec3(0.0f);
        }
        if (glm::length(b->angularVelocity) < angularVelocityThreshold) {
            b->angularVelocity = glm::vec3(0.0f);
        }

        // Positional correction to remove penetration
        float penetrationDepth = glm::max(info.penetrationDepth - penetrationSlack, 0.0f);
        glm::vec3 correction = (penetrationDepth * correctionFactor) * info.normal;
        a->position -= correction * a->inverseMass;
        b->position += correction * b->inverseMass;
    }
}
void System::update(float dt){
    for (auto& body : bodies) {
        spdlog::debug("Force generators size: {}", force_generators.size());
        for (auto force_generator: force_generators){
            spdlog::debug("Applying force generator {}", force_generator->toString());
            force_generator->apply(body, dt);
        }
    }
    // Then, resolve collisions
    for (int i = 0; i < bodies.size(); i++){
        for (int j = 0; j < static_bodies.size(); j++){
            resolveCollision(bodies[i], static_bodies[j]);
        }
    }
    for (int i = 0; i < bodies.size(); i++){
        for (int j = i + 1; j < bodies.size(); j++){
            resolveCollision(bodies[i], bodies[j]);
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
void System::addStaticBody(RigidBody* body){
    spdlog::debug("Adding static body...");
    static_bodies.push_back(std::move(body));
    spdlog::debug("Number of static bodies:{}", static_bodies.size());
}
const std::vector<RigidBody*> System::getBodies() const {
    return bodies;
}
const std::vector<RigidBody*> System::getStaticBodies() const {
    return static_bodies;
}
System::System(float dt, ODESolver *solver): dt(dt), solver(std::move(solver)){
    spdlog::debug("Adding Gravity.");
    force_generators.push_back(new Gravity());
    // force_generators.push_back(new AirResistance());
}
System::System(){}
}