
#include <graphics/Simulation.hpp>

namespace aun {

void Simulation::initSDL() {

    window = SDL_CreateWindow("Rigid Body Physics Simulation", WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
    if (window == nullptr) {
        spdlog::error("Window could not be created! SDL_Error: {}", SDL_GetError());
        exit(1);
    }
    
    spdlog::info("SDL initialised");
}

void Simulation::initOpenGL() {
    glContext = SDL_GL_CreateContext(window);
    if (glContext == nullptr) {
        spdlog::error("OpenGL context could not be created! SDL Error: {}", SDL_GetError());
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
        spdlog::error("SDL could not initialize! SDL_Error: {}", SDL_GetError());
        exit(1);
    }

    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        spdlog::error("Error initializing GLEW! {}", reinterpret_cast<const char*>(glewGetErrorString(glewError)));
        exit(1);
    }
    char * vertex_source = VERTEX_SHADER_PATH;
    char * fragment_source = FRAGMENT_SHADER_PATH;
    shaderProgram = createShaderProgram(vertex_source, fragment_source);
    spdlog::info("Shader program compiled and linked sucessfully.");
    glViewport(0, 0, 800, 600);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    spdlog::info("OpenGL succesfully initialised.");
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
        // Handle mouse motion
        if (event.type == SDL_EVENT_MOUSE_MOTION) {
            inputManager.handleMouseMovement(event.motion.x, event.motion.y, camera);
        }

    }
}

void Simulation::update() {
    system->update(1.0f / 60.0f);
}

void Simulation::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render your objects here
    // Render all game objects

    glm::mat4 view = camera.getView();
    glm::mat4 projection = camera.getProjection();
    std::cout << "View matrix in render";
    log_mat4(view);
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    GLuint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
    glUniform3f(lightPosLoc, 1.2f, 1.0f, 2.0f);  // Example light position

    GLuint lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
    glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);  // White light

    GLuint colorTopLoc = glGetUniformLocation(shaderProgram, "colorTop");
    glUniform3f(colorTopLoc, 0.0f, 0.5f, 1.0f);  // Example top color (sky blue)

    GLuint colorBottomLoc = glGetUniformLocation(shaderProgram, "colorBottom");
    glUniform3f(colorBottomLoc, 0.8f, 0.4f, 0.1f);  // Example bottom color (orange)

    for (const auto& body : system->getBodies()) {
        body->geometry->graphicsModel->render(body->getTransformMatrix());
    }
    
    SDL_GL_SwapWindow(window);
}


Simulation::Simulation() {}
Simulation::Simulation(bool running): running(running) {
    initSDL();
    initOpenGL();
    camera = Camera(WIDTH, HEIGHT);
    inputManager = InputManager();
}

Simulation::~Simulation() {
    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
void Simulation::setSystem(std::unique_ptr<System> system){
    this->system = std::move(system);
    spdlog::debug("Setting system shader programs...");
    for (auto &body: this->system->getBodies()){
        body->geometry->graphicsModel->setShaderProgram(&shaderProgram);
    }
}
void Simulation::run() {
    running = true;
    Uint64 lastTime = SDL_GetTicks();
    const Uint64 frameDelay = 1000 / 60; // 60 FPS

    while (running) {
        Uint64 frameStart = SDL_GetTicks();

        handleEvents();
        inputManager.handleInput(1.0f / 60.0, camera);
        update();

        render();

        Uint64 frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}


}