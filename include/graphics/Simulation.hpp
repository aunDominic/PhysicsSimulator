// Simulation.hpp
#pragma once
#include <SDL3/SDL.h>
#include <GL/glew.h>
#include <physics/System.hpp>
#include <graphics/InputManager.hpp>
#include <iostream>
#include <graphics/utils.hpp>
#include <graphics/Camera.hpp>
#include "spdlog/spdlog.h"


namespace aun {
#define WIDTH  800
#define HEIGHT 600
#define FRAGMENT_SHADER_PATH "/Users/dominic/Desktop/CS Projects/Rigid Body Physics /include/graphics/FragmentShader.glsl"
#define VERTEX_SHADER_PATH "/Users/dominic/Desktop/CS Projects/Rigid Body Physics /include/graphics/VertexShader.glsl"

class Simulation {
private:
    SDL_Window* window;
    SDL_GLContext glContext;
    std::unique_ptr<System> system;
    InputManager inputManager;
    GLuint shaderProgram;
    Camera camera;
    
    bool running;

    void initSDL();
    void initOpenGL();
    void handleEvents();
    void update();
    void render();

public:
    Simulation();
    Simulation(bool running);
    ~Simulation();
    void run();
    void setSystem(std::unique_ptr<System> system);
};

}