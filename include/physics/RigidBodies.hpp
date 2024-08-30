#pragma once

#include <physics/RigidBody.hpp>
namespace aun{
class Ball : public RigidBody{
    void moveUp();
    Ball();
};
class TestRigidBody : public RigidBody {
public:
     TestRigidBody(glm::vec3 pos, glm::quat ori, glm::vec3 vel, glm::vec3 angVel);
     TestRigidBody();
     void derivativeEvaluation(const glm::vec3& position, const glm::quat& orientation,
                              const glm::vec3& velocity, const glm::vec3& angularVelocity,
                              glm::vec3& outPos, glm::quat& outOri,
                              glm::vec3& outVel, glm::vec3& outAng) override;
};   
}