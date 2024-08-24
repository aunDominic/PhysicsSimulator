#include <physics/Solvers.hpp>
namespace aun{

void RungeKuttaSolver::solve(RigidBody& body, float dt){
    glm::vec3 k1_pos, k2_pos, k3_pos, k4_pos;
    glm::quat k1_ori, k2_ori, k3_ori, k4_ori;
    glm::vec3 k1_vel, k2_vel, k3_vel, k4_vel;
    glm::vec3 k1_ang, k2_ang, k3_ang, k4_ang;

    glm::vec3 original_pos = body.position;
    glm::quat original_ori = body.orientation;
    glm::vec3 original_vel = body.velocity;
    glm::vec3 original_ang = body.angularVelocity;

    // First step
    body.derivativeEvaluation(original_pos, original_ori, original_vel, original_ang,
                                k1_pos, k1_ori, k1_vel, k1_ang);

    // Second step
    body.derivativeEvaluation(original_pos + k1_pos * dt * 0.5f,
                                original_ori + k1_ori * dt * 0.5f,
                                original_vel + k1_vel * dt * 0.5f,
                                original_ang + k1_ang * dt * 0.5f,
                                k2_pos, k2_ori, k2_vel, k2_ang);

    // Third step
    body.derivativeEvaluation(original_pos + k2_pos * dt * 0.5f,
                                original_ori + k2_ori * dt * 0.5f,
                                original_vel + k2_vel * dt * 0.5f,
                                original_ang + k2_ang * dt * 0.5f,
                                k3_pos, k3_ori, k3_vel, k3_ang);

    // Fourth step
    body.derivativeEvaluation(original_pos + k3_pos * dt,
                                original_ori + k3_ori * dt,
                                original_vel + k3_vel * dt,
                                original_ang + k3_ang * dt,
                                k4_pos, k4_ori, k4_vel, k4_ang);

    // Update state
    body.position += (k1_pos + 2.0f * k2_pos + 2.0f * k3_pos + k4_pos) * (dt / 6.0f);
    body.orientation += (k1_ori + 2.0f * k2_ori + 2.0f * k3_ori + k4_ori) * (dt / 6.0f);
    body.velocity += (k1_vel + 2.0f * k2_vel + 2.0f * k3_vel + k4_vel) * (dt / 6.0f);
    body.angularVelocity += (k1_ang + 2.0f * k2_ang + 2.0f * k3_ang + k4_ang) * (dt / 6.0f);

    body.orientation = glm::normalize(body.orientation);

    // Reset forces and torques
    body.force = glm::vec3(0);
    body.torque = glm::vec3(0);
}

}