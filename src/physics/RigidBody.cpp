#include <physics/RigidBody.hpp>
#include <spdlog/spdlog.h>
#include <graphics/utils.hpp>
namespace aun{        
void RigidBody::applyForce(const glm::vec3& force, const glm::vec3& point) {
    this->force += force;
    
    glm::vec3 torque = glm::cross(point - position, force);
    glm::vec3 bodyTorque = glm::inverse(orientation) * torque;
    glm::vec3 bodyAngularAcceleration = inverseInertiaTensor * bodyTorque;
    this->torque += orientation * bodyAngularAcceleration;
}

// Calculates the derivatives of variables that changes over time. 
void RigidBody::derivativeEvaluation(const glm::vec3& position, const glm::quat& orientation,
                                const glm::vec3& linearVelocity, const glm::vec3& angularVelocity,
                                glm::vec3& positionDerivative, glm::quat& orientationDerivative,
                                glm::vec3& linearVelocityDerivative, glm::vec3& angularVelocityDerivative) {
    positionDerivative = linearVelocity;
    orientationDerivative = 0.5f * glm::quat(0, angularVelocity.x, angularVelocity.y, angularVelocity.z) * orientation;
    linearVelocityDerivative = force * inverseMass;
    angularVelocityDerivative = glm::inverse(orientation) * (inverseInertiaTensor * (orientation * torque));
}
glm::mat4 RigidBody::getTransformMatrix() const {
    
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), this->position);

    // Apply rotation (assuming rigidBody.rotation is a quaternion)
    transform *= glm::mat4_cast(this->orientation);

 
    // Apply scaling (assuming rigidBody.scale is a vec3)
    // transform = glm::scale(transform, rigidBody.scale);
    spdlog::info("Got transformation matrix.");
    log_mat4(transform);
    return transform;
}
}