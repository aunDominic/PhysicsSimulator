#include <physics/RigidBodies.hpp>
namespace aun{
// Define a simple RigidBody class for testing

TestRigidBody::TestRigidBody(){}
TestRigidBody::TestRigidBody(glm::vec3 pos, glm::quat ori, glm::vec3 vel, glm::vec3 angVel)
        : RigidBody(pos, ori, vel, angVel) {}

void TestRigidBody::derivativeEvaluation(const glm::vec3& position, const glm::quat& orientation,
                              const glm::vec3& velocity, const glm::vec3& angularVelocity,
                              glm::vec3& outPos, glm::quat& outOri,
                              glm::vec3& outVel, glm::vec3& outAng)  {
        // Simple harmonic oscillator with omega^2 = k / m
        float omega = 1.0f; // Natural frequency (for simplicity, set to 1)
        outPos = velocity;
        outOri = glm::quat(0, angularVelocity.x, angularVelocity.y, angularVelocity.z);
        outVel = -omega * omega * position; // Force = -k * x, k/m = omega^2
        outAng = glm::vec3(0.0f); // No angular dynamics in this test
    }

}