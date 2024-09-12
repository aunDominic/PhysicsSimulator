#include <physics/StaticBody.hpp>
namespace aun{

StaticBody::StaticBody() : RigidBody(){
    mass = std::numeric_limits<float>::infinity();
    inverseMass = 0.0f;
    inertiaTensor = glm::mat3(0.0f);
    inverseInertiaTensor = glm::mat3(0.0f);
}
StaticBody::StaticBody(glm::vec3 position) : RigidBody(position){
    mass = std::numeric_limits<float>::infinity();
    inverseMass = 0.0f;
    inertiaTensor = glm::mat3(0.0f);
    inverseInertiaTensor = glm::mat3(0.0f);
};


// Override methods that shouldn't affect static bodies
void StaticBody::applyForce(const glm::vec3& force, const glm::vec3& point) {
    // Dont apply any force unto a static body.
}

void StaticBody::derivativeEvaluation(const glm::vec3& position, const glm::quat& orientation,
                            const glm::vec3& linearVelocity, const glm::vec3& angularVelocity,
                            glm::vec3& positionDerivative, glm::quat& orientationDerivative,
                            glm::vec3& linearVelocityDerivative, glm::vec3& angularVelocityDerivative) {
    // Set all derivatives to zero
    positionDerivative = glm::vec3(0.0f);
    orientationDerivative = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    linearVelocityDerivative = glm::vec3(0.0f);
    angularVelocityDerivative = glm::vec3(0.0f);
}

}