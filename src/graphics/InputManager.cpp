#include <graphics/InputManager.hpp>

namespace aun{
void InputManager::updateKeyState(const SDL_KeyboardEvent& keyEvent) {
    keyStates[keyEvent.key] = (keyEvent.type == SDL_EVENT_KEY_DOWN);
}

bool InputManager::isKeyPressed(SDL_Keycode key) const {
    auto it = keyStates.find(key);
    return (it != keyStates.end() && it->second);
}

void InputManager::loadInputConfig(const std::string& filePath) {
    std::ifstream file(filePath);
    nlohmann::json config;
    file >> config;

    for (const auto& [actionName, inputMapping] : config.items()) {
        actionMapping[actionName] = static_cast<SDL_Keycode>(inputMapping);
    }
} 
void InputManager::handleInput(float dt){
    // TODO: implement handleInput
}
}