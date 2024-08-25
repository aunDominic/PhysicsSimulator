#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <physics/Geometry.hpp>

namespace aun{
    class RigidBody {
    public:
        // Geometric Properties
        std::unique_ptr<Geometry> geometry;

        // Mass properties
        float mass;
        float inverseMass;
        glm::mat3 inertiaTensor;         // TODO: Calculate inertiaTensor
        glm::mat3 inverseInertiaTensor;

        // Position and orientation
        glm::vec3 position;                 // Center of mass
        glm::quat orientation;              

        // Linear dynamics
        glm::vec3 velocity;                 
        glm::vec3 force;                    

        // Angular dynamics
        glm::vec3 angularVelocity;   
        glm::vec3 torque;

        // ... constructors and other methods ...
        RigidBody();
        ~RigidBody() = default;
        void applyForce(const glm::vec3& force, const glm::vec3& point= glm::vec3(0,0,0));

        void derivativeEvaluation(const glm::vec3& position, const glm::quat& orientation,
                                const glm::vec3& linearVelocity, const glm::vec3& angularVelocity,
                                glm::vec3& positionDerivative, glm::quat& orientationDerivative,
                                glm::vec3& linearVelocityDerivative, glm::vec3& angularVelocityDerivative);
        glm::mat4 getTransformMatrix() const;
    };
}