#pragma once 

#include <unordered_map>
#include <SDL3/SDL.h>
#include <nlohmann/json.hpp>
#include <fstream>  
#include <graphics/Camera.hpp>  
#include <spdlog/spdlog.h>
#include <string>
#include <iostream>
#include <physics/RigidBody.hpp>
#include <graphics/Inputable.hpp>

namespace aun{
class InputManager {
private:
    std::unordered_map<SDL_Keycode, bool> keyStates;
    std::unordered_map<std::string, SDL_Keycode> actionMapping;
    float lastX, lastY;
    bool firstMouse;
    Inputable *toReceive;
public:
    void updateKeyState(const SDL_KeyboardEvent& keyEvent);
    bool isKeyPressed(SDL_Keycode key) const;
    bool isKeyPressed(const std::string& action) const;
    void loadInputConfig(const std::string& filePath);
    void handleInput(float dt);
    void handleInput(float dt, Camera& camera);
    void handleInput(float dt, RigidBody *body);
    void setInputable(Inputable *object);


    void handleMouseMovement(int xpos, int ypos, Camera& camera);
    InputManager();
    // Potentially add methods for other input types (mouse, gamepad, etc.)
};

}