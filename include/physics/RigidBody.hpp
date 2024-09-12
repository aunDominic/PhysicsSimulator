#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <physics/Geometry.hpp>
#include <graphics/Inputable.hpp>

namespace aun{
    class RigidBody: public Inputable{
    public:
        // Mass properties
        float mass = 5.0f; // Default mass = 5kg
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
        // Geometric Properties
        Geometry *geometry;

        // ... constructors and other methods ...
        RigidBody();
        RigidBody(glm::vec3 position);
        RigidBody(glm::quat ori);
        RigidBody(glm::vec3 position, glm::quat orientation, glm::vec3 velocity, glm::vec3 angularVelocity);
        ~RigidBody() = default;
        virtual void applyForce(const glm::vec3& force, const glm::vec3& point= glm::vec3(0,0,0));

        virtual void derivativeEvaluation(const glm::vec3& position, const glm::quat& orientation,
                                const glm::vec3& linearVelocity, const glm::vec3& angularVelocity,
                                glm::vec3& positionDerivative, glm::quat& orientationDerivative,
                                glm::vec3& linearVelocityDerivative, glm::vec3& angularVelocityDerivative);
        glm::mat4 getTransformMatrix() const;
        void setGeometry(Geometry* geo);
        glm::vec3 getPosition() {return position;};


        // Inputs
        void processKeyboard(const std::string &direction, float deltaTime) override;
        void processMouseMovement(float xoffset, float yoffset) override;

    };
}