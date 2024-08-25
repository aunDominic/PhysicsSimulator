
#include <graphics/Simulation.hpp>
#include <physics/Solvers.hpp>
#include <physics/RigidBodies.hpp>
#include <graphics/SphereModel.hpp>
#include <physics/Sphere.hpp>
#include <graphics/utils.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>  // for file-based logging
#include <spdlog/sinks/stdout_color_sinks.h>  // for console logging
void init_multi_sink_logger(); 



int main(int argc, char* args[]) {
    init_multi_sink_logger();

    std::unique_ptr<aun::ODESolver> solver = std::make_unique<aun::EulerSolver>(); 
    std::unique_ptr<aun::System> BallSystem = std::make_unique<aun::System>(1000.0f/60.0f, std::move(solver));
    if (BallSystem == nullptr) {
        spdlog::error("BallSystem is null!");
    }
    aun::Simulation simulation(true); // Passing a pointer to BallSystem

    spdlog::debug("Creating Rigid Body...\n");
    std::unique_ptr<aun::RigidBody> body = std::make_unique<aun::RigidBody>(); 

    spdlog::debug("Creating Geometry...\n");
    std::unique_ptr<aun::Geometry> geometry = std::make_unique<aun::Sphere>(); 

    spdlog::debug("Creating Graphics model...\n");
    std::unique_ptr<aun::SphereModel> graphicsModel = std::make_unique<aun::SphereModel>(); 

    spdlog::debug("Sphere coordinates:");
    aun::log_mat4(body->getTransformMatrix());
    spdlog::debug("Moving Graphics model...\n");
    geometry->graphicsModel = std::move(graphicsModel);
   
    spdlog::debug("Moving Geometry...\n");
    body->geometry = std::move(geometry);

    spdlog::debug("Moving Rigid Body...\n");
    BallSystem->addRigidBody(std::move(body));
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
