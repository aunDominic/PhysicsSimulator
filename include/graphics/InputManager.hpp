#pragma once 

#include <unordered_map>
#include <SDL3/SDL.h>
#include <nlohmann/json.hpp>
#include <fstream>    

namespace aun{
class InputManager {
private:
    std::unordered_map<SDL_Keycode, bool> keyStates;
    std::unordered_map<std::string, SDL_Keycode> actionMapping;

public:
    void updateKeyState(const SDL_KeyboardEvent& keyEvent);
    bool isKeyPressed(SDL_Keycode key) const;
    void loadInputConfig(const std::string& filePath);
    void handleInput(float dt);
    // Potentially add methods for other input types (mouse, gamepad, etc.)
};

}