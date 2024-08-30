#include <graphics/InputManager.hpp>

namespace aun{
InputManager::InputManager(){
    InputManager::loadInputConfig("/Users/dominic/Desktop/CS Projects/Rigid Body Physics /input_config.json");
    spdlog::debug("Inputs are:");
    for (const auto&[key, val]: actionMapping){
        std::cout << key << " " << val;
    }
    firstMouse = true;
}
void InputManager::updateKeyState(const SDL_KeyboardEvent& keyEvent) {
    keyStates[keyEvent.key] = (keyEvent.type == SDL_EVENT_KEY_DOWN);
}

bool InputManager::isKeyPressed(SDL_Keycode key) const {
    auto it = keyStates.find(key);
    return (it != keyStates.end() && it->second);
}
bool InputManager::isKeyPressed(const std::string& action) const {
    auto it = actionMapping.find(action);
    return (it != actionMapping.end() && isKeyPressed(it->second));
}

void InputManager::loadInputConfig(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        spdlog::error("Failed to open input config file.");
    }
    nlohmann::json config;
    file >> config;

    for (const auto& action : config.items()) {
        std::string actionName = action.key();
        std::string keyName = action.value().get<std::string>();

        SDL_Keycode keyCode = SDL_GetKeyFromName(keyName.c_str());

        if (keyCode != SDLK_UNKNOWN) {
            actionMapping[actionName] = keyCode;
        } else {
            spdlog::error("Unknown key name: {} in action {}", keyName, actionName);
        }    
    }
    spdlog::info("Succesfully loaded config input");
} 
void InputManager::handleInput(float dt){
    // TODO: implement handleInput
}
void InputManager::handleInput(float dt, Camera& camera) {
    if (isKeyPressed("FORWARD")) {
        spdlog::debug("Input FORWARD is detected.");
        camera.processKeyboard("FORWARD", dt);
    }
    if (isKeyPressed("BACKWARD")) {
        camera.processKeyboard("BACKWARD", dt);
    }
    if (isKeyPressed("LEFT")) {
        camera.processKeyboard("LEFT", dt);
    }
    if (isKeyPressed("RIGHT")) {
        camera.processKeyboard("RIGHT", dt);
    }
    if (isKeyPressed("UP")){
        camera.processKeyboard("UP", dt);
    }
    if (isKeyPressed("DOWN")){
        camera.processKeyboard("DOWN", dt);
    }
}
void InputManager::handleInput(float dt, RigidBody *body){
    if (isKeyPressed("FORWARD")) {
        spdlog::debug("Input FORWARD is detected.");
        body->processKeyboard("FORWARD", dt);
    }
    if (isKeyPressed("BACKWARD")) {
        body->processKeyboard("BACKWARD", dt);
    }
    if (isKeyPressed("LEFT")) {
        body->processKeyboard("LEFT", dt);
    }
    if (isKeyPressed("RIGHT")) {
        body->processKeyboard("RIGHT", dt);
    }
    if (isKeyPressed("UP")){
        body->processKeyboard("UP", dt);
    }
    if (isKeyPressed("DOWN")){
        body->processKeyboard("DOWN", dt);
    }
}
void InputManager::handleMouseMovement(int xpos, int ypos, Camera& camera) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    spdlog::debug("xpos: {}, lastX: {}", xpos, lastX);
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Inverted y-axis for natural feel

    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(xoffset, yoffset);
}

}