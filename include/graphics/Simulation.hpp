// Simulation.hpp
#pragma once
#include <SDL3/SDL.h>
#include <GL/glew.h>
#include <physics/System.hpp>
#include <graphics/InputManager.hpp>
#include <iostream>
#include <graphics/utils.hpp>

namespace aun {

class Simulation {
private:
    SDL_Window* window;
    SDL_GLContext glContext;
    System system;
    InputManager inputManager;
    GLuint shaderProgram;

    bool running;

    void initSDL();
    void initOpenGL();
    void handleEvents();
    void update();
    void render();

public:
    Simulation();
    Simulation(System &system, bool running);
    ~Simulation();
    void run();
};

}