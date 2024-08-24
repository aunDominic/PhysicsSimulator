
#include <graphics/Simulation.hpp>
#include "spdlog/spdlog.h"

#define FRAGMENT_SHADER_PATH "/Users/dominic/Desktop/CS Projects/Rigid Body Physics /include/graphics/FragmentShader.glsl"
#define VERTEX_SHADER_PATH "/Users/dominic/Desktop/CS Projects/Rigid Body Physics /include/graphics/VertexShader.glsl"
namespace aun {
void Simulation::initSDL() {

    window = SDL_CreateWindow("Rigid Body Physics Simulation", 800, 600, SDL_WINDOW_OPENGL);
    if (window == nullptr) {
        spdlog::error("Window could not be created! SDL_Error: {}\n", SDL_GetError());
        exit(1);
    }
    
    spdlog::info("SDL initialised");
}

void Simulation::initOpenGL() {
    glContext = SDL_GL_CreateContext(window);
    if (glContext == nullptr) {
        spdlog::error("OpenGL context could not be created! SDL Error: {}\n", SDL_GetError());
        exit(1);
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);


    const GLubyte* version = glGetString(GL_VERSION);
    spdlog::info("OpenGL version: {}", reinterpret_cast<const char *>(version));

    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    spdlog::info("GLSL version: {}",  reinterpret_cast<const char *>(glslVersion));
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        spdlog::error("SDL could not initialize! SDL_Error: {}\n", SDL_GetError());
        exit(1);
    }

    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        spdlog::error("Error initializing GLEW! {}\n", reinterpret_cast<const char*>(glewGetErrorString(glewError)));
        exit(1);
    }
    char * vertex_source = VERTEX_SHADER_PATH;
    char * fragment_source = FRAGMENT_SHADER_PATH;
    shaderProgram = createShaderProgram(vertex_source, fragment_source);
    spdlog::info("Shader program compiled and linked sucessfully.\n");
    for (auto &body: system.getBodies()){
        body->geometry->graphicsModel->setShaderProgram(&shaderProgram);
    }
    glViewport(0, 0, 800, 600);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
}

void Simulation::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            running = false;
        }
        if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
            inputManager.updateKeyState(event.key);
        }
    }
}

void Simulation::update() {
    system.update(1.0f / 60.0f);
}

void Simulation::render() {
    spdlog::info("Rendering graphics models...");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render your objects here
    // Render all game objects
    for (const auto& body : system.getBodies()) {
        spdlog::info("Rendering GeometryType{}", static_cast<int>(body->geometry->getType()));

        body->geometry->graphicsModel->render(body->getTransformMatrix());
    }
    
    SDL_GL_SwapWindow(window);
}


Simulation::Simulation() {}
Simulation::Simulation(System &system, bool running): running(running) {
    initSDL();
    initOpenGL();
    this->system = std::move(system);
}

Simulation::~Simulation() {
    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Simulation::run() {
    running = true;
    Uint64 lastTime = SDL_GetTicks();
    const Uint64 frameDelay = 1000 / 60; // 60 FPS

    while (running) {
        Uint64 frameStart = SDL_GetTicks();

        handleEvents();
        inputManager.handleInput(1.0f / 60.0f);
        update();

        render();

        Uint64 frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}


}