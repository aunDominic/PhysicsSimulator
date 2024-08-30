#include <physics/Solvers.hpp>
#include <physics/RigidBodies.hpp>
#include <iostream>
#include <fstream>

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

void testEulerSolver() {
    float dt = 0.01f;
    float t = 0.0f;
    float totalTime = 20.0f;

    // Initial conditions
    glm::vec3 initial_position(1.0f, 0.0f, 0.0f);
    glm::vec3 initial_velocity(0.0f, 1.0f, 0.0f);
    glm::quat initial_orientation(1.0f, 0.0f, 0.0f, 0.0f); // No rotation for this test
    glm::vec3 initial_angularVelocity(0.0f, 0.0f, 0.0f);    // No angular velocity

    // Create the test rigid body
    TestRigidBody body(initial_position, initial_orientation, initial_velocity, initial_angularVelocity);

    // Create the Euler solver
    EulerSolver solver;
    // Open a file to write the results
    std::ofstream outputFile("euler_solver_results.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return;
    }
     // Write header
    outputFile << "Time, Analytical_Pos_X, Analytical_Pos_Y, Analytical_Pos_Z, "
               << "Simulated_Pos_X, Simulated_Pos_Y, Simulated_Pos_Z, "
               << "Error_X, Error_Y, Error_Z\n";


    // Simulate and collect data
    while (t < totalTime) {
        // Analytical solution
        glm::vec3 analytical_pos = analyticalSolution(t, 1.0f, initial_position, initial_velocity);

        // Solve with Euler
        solver.solve(body, dt);
    
        // Calculate the error
        glm::vec3 error = glm::abs(body.position - analytical_pos);

        // Output the time, analytical position, simulated position, and error
        outputFile << "Time: " << t << ", Analytical Pos: (" << analytical_pos.x << ", " << analytical_pos.y << ", " << analytical_pos.z << ")"
                  << ", Simulated Pos: (" << body.position.x << ", " << body.position.y << ", " << body.position.z << ")"
                  << ", Error: (" << error.x << ", " << error.y << ", " << error.z << ")" << std::endl;

        // Increment time
        t += dt;
    }
      // Close the file
    outputFile.close();
    std::cout << "Results have been written to euler_solver_results.txt" << std::endl;
}

}