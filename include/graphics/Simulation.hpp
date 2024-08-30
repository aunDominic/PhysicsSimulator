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
#include <graphics/GridModel.hpp>
#include <physics/CollisionDetector.hpp>
#include <graphics/SeparatingPlaneModel.hpp>

namespace aun {
#define WIDTH  800
#define HEIGHT 600
#define FRAGMENT_SHADER_PATH "/Users/dominic/Desktop/CS Projects/Rigid Body Physics /include/graphics/FragmentShader.glsl"
#define VERTEX_SHADER_PATH "/Users/dominic/Desktop/CS Projects/Rigid Body Physics /include/graphics/VertexShader.glsl"
#define GRID_VERTEX_SHADER_PATH "/Users/dominic/Desktop/CS Projects/Rigid Body Physics /include/graphics/GridVertexShader.glsl"
#define GRID_FRAGMENT_SHADER_PATH "/Users/dominic/Desktop/CS Projects/Rigid Body Physics /include/graphics/GridFragmentShader.glsl"

class Simulation {
private:
    SDL_Window* window;
    SDL_GLContext glContext;
    System *system;
    InputManager inputManager;
    GLuint shaderProgram;
    GLuint gridShaderProgram;
    Camera camera;
    GridModel gridModel;
    CollisionDetector *collisionDetector = new SATDetector();
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
    void setSystem(System *system);
};

}