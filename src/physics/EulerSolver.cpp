#include <physics/Solvers.hpp>
namespace aun{

void EulerSolver::solve(RigidBody& body, float dt){
    glm::vec3 positionDerivative, linearVelocityDerivative, angularVelocityDerivative;
    glm::quat orientationDerivative;

    body.derivativeEvaluation(body.position, body.orientation, body.velocity, body.angularVelocity,
                            positionDerivative, orientationDerivative,
                            linearVelocityDerivative, angularVelocityDerivative);

    body.position += positionDerivative * dt;
    body.orientation += orientationDerivative * dt;
    body.velocity += linearVelocityDerivative * dt;
    body.angularVelocity += angularVelocityDerivative * dt;

    body.orientation = glm::normalize(body.orientation);

    // Reset forces and torques
    body.force = glm::vec3(0);
    body.torque = glm::vec3(0);
}

}