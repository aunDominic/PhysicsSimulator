#include <physics/RigidBody.hpp>
#include <spdlog/spdlog.h>
#include <graphics/utils.hpp>
namespace aun{       
RigidBody::RigidBody(){
    position = glm::vec3(0);
    orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);    
} 
RigidBody::RigidBody(glm::vec3 position) : position(position){
    orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
}
RigidBody::RigidBody(glm::quat ori) : orientation(ori){
    position = glm::vec3(0);

}
RigidBody::RigidBody(glm::vec3 position, glm::quat orientation, 
glm::vec3 velocity, glm::vec3 angularVelocity) : position(position), orientation(orientation), 
velocity(velocity), angularVelocity(angularVelocity){

};

void RigidBody::applyForce(const glm::vec3& force, const glm::vec3& point) {
    spdlog::debug("Applying force:{}, at position:{}", vec3_to_string(force), vec3_to_string(point));
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
    spdlog::debug("Called in RigidBody position:{}, orientation:", vec3_to_string(position));
    log_mat4(glm::mat4_cast(orientation));
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), this->position);

    // Apply rotation (assuming rigidBody.rotation is a quaternion)
    transform *= glm::mat4_cast(this->orientation);

 
    // Apply scaling (assuming rigidBody.scale is a vec3)
    // transform = glm::scale(transform, rigidBody.scale);
    return transform;
}

void RigidBody::setGeometry(Geometry *geo) {
    spdlog::debug("Setting Geometry for RigidBody...");

    geometry = std::move(geo);
    geometry->position = &position;
    geometry->orientation = &orientation;
    geometry->graphicsModel->setNormalVertices(geometry->getFaceNormalVertices(5.0f));
    geometry->graphicsModel->initNormalBuffers();
}
void RigidBody::processKeyboard(const std::string &direction, float deltaTime) {
    spdlog::debug("Processing keyboard inputs for RigidBody");
    float velocity = movementSpeed * deltaTime;
    glm::vec3 front = glm::vec3(0,0,1);
    glm::vec3 right = glm::vec3(1,0,0);
    glm::vec3 up    = glm::vec3(0,1,0);
    if (direction == "FORWARD")
        position += front * velocity;
    else if (direction == "BACKWARD")
        position -= front * velocity;
    else if (direction == "LEFT")
        position -= right * velocity;
    else if (direction == "RIGHT")
        position += right * velocity;
    else if (direction == "UP")
        position += up * velocity;
    else if (direction == "DOWN")
        position -= up * velocity;
}
void RigidBody::processMouseMovement(float xoffset, float yoffset) {
    
}
}