
#include <graphics/Simulation.hpp>
#include <physics/Solvers.hpp>
#include <physics/RigidBodies.hpp>
#include <graphics/SphereModel.hpp>
#include <physics/Sphere.hpp>
#include <graphics/utils.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>  // for file-based logging
#include <spdlog/sinks/stdout_color_sinks.h>  // for console logging
#include <physics/StaticBody.hpp>
#include <physics/BoxGeometry.hpp>
void init_multi_sink_logger(); 



int main(int argc, char* args[]) {
    init_multi_sink_logger();

    auto solver = new aun::RungeKuttaSolver(); 
    auto BallSystem = new aun::System(1000.0f/60.0f, std::move(solver));
    if (BallSystem == nullptr) {
        spdlog::error("BallSystem is null!");
    }
    aun::Simulation simulation(true); // Passing a pointer to BallSystem


    auto wallLeft = new aun::StaticBody(glm::vec3(-50,0,0));
    auto wallRight = new aun::StaticBody(glm::vec3(50,0,0));
    auto wallFront = new aun::StaticBody(glm::vec3(0,0,50));
    auto wallBack = new aun::StaticBody(glm::vec3(0,0,-50));
    auto ceiling = new aun::StaticBody(glm::vec3(0,50,0));


    auto floor = new aun::StaticBody(glm::vec3(0,-2,0));
    spdlog::debug("Creating Rigid Body...\n");
    // Creating a quaternion for a rotation about the Y-axis

    // Applying the quaternion to the RigidBody
    auto body = new aun::RigidBody(glm::normalize(glm::quat(1.0f, 0.2f, 0.3f, 0.5)));
    auto body_2 = new aun::RigidBody(glm::vec3(0, 40, 0));
    auto body_3 = new aun::RigidBody(glm::vec3(2, 10, 10));
    auto body_4 = new aun::RigidBody(glm::vec3(0, 10, 0));

    spdlog::debug("BOx coordinates:");
    aun::log_mat4(body->getTransformMatrix());
   
    spdlog::debug("Creating Geometry...\n");
    floor->setGeometry(new aun::BoxGeometry(glm::vec3(0),100, 1, 100));
    wallLeft->setGeometry(new aun::BoxGeometry(glm::vec3(0),1, 100, 100));
    wallRight->setGeometry(new aun::BoxGeometry(glm::vec3(0),1, 100, 100));
    wallFront->setGeometry(new aun::BoxGeometry(glm::vec3(0),100, 100, 1));
    wallBack->setGeometry(new aun::BoxGeometry(glm::vec3(0),100, 100, 1));
    ceiling->setGeometry(new aun::BoxGeometry(glm::vec3(0),100, 1, 100));
<<<<<<< HEAD
    body_3->setGeometry(new aun::BoxGeometry(glm::vec3(0),10, 5, 10));
    body_4->setGeometry(new aun::BoxGeometry(glm::vec3(0), 10, 5, 10));
    body_2->setGeometry(new aun::BoxGeometry(glm::vec3(0),10, 5, 20));
=======
    body_2->setGeometry(new aun::BoxGeometry(glm::vec3(0),10, 30, 10));
>>>>>>> e1d1e69 (Dev testing)
    body->setGeometry(new aun::BoxGeometry(glm::vec3(0),10,10,10));
    body->angularVelocity = glm::vec3(10,10,10);
    spdlog::debug("Moving Rigid Body...\n");
    BallSystem->addRigidBody(std::move(body));
    BallSystem->addStaticBody(std::move(floor));
    BallSystem->addStaticBody(std::move(wallLeft));
    BallSystem->addStaticBody(std::move(wallRight));
    BallSystem->addStaticBody(std::move(wallBack));
    BallSystem->addStaticBody(std::move(wallFront));
    BallSystem->addStaticBody(std::move(ceiling));
    BallSystem->addRigidBody(std::move(body_2));
    BallSystem->addRigidBody(std::move(body_3));
    BallSystem->addRigidBody(std::move(body_4));

    simulation.setSystem(std::move(BallSystem));
    simulation.run();
    return 0;
}

void init_multi_sink_logger() {
    try {
        // Create a console sink
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

        // Create a file sink
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/project.log", true);

        // Create a multi-sink logger (tee_sink combines multiple sinks)
        std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};
        auto logger = std::make_shared<spdlog::logger>("multi_sink", sinks.begin(), sinks.end());

        // Set log level (optional)
        logger->set_level(spdlog::level::debug);  // or spdlog::level::debug, etc.

        // Set the pattern (optional)
        logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");

        // Register it so that you can use it globally
        spdlog::set_default_logger(logger);

        // Optionally, register the logger by name if you want to use multiple loggers
        spdlog::register_logger(logger);
    }
    catch (const spdlog::spdlog_ex& ex) {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
}
