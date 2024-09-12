#include <graphics/Camera.hpp>
namespace aun{
glm::mat4 Camera::getView(){
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    return view;
}
glm::mat4 Camera::getProjection(){
    glm::mat4 projection = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 100.0f);
    return projection;
}
Camera::Camera(){

}
Camera::Camera(int width, int height) : width(width), height(height), Inputable(10.0f, 0.5f){   
    cameraPos = glm::vec3(0.0f, 0.0f, 50.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -50.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    fov = 45.0f;
    worldUp = cameraUp;
    yaw = -90.0f;
    pitch = 0.0f;
    spdlog::debug("Camera width: {}, Camera height: {}", width, height);
}
void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
    cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
    spdlog::debug("New Camera view matrix:");
    log_mat4(getView());
    spdlog::debug("New Camera projection matrix:");
    log_mat4(getProjection());
}
void Camera::processKeyboard(const std::string &direction, float deltaTime) {
    spdlog::debug("Processing Camera keyboard inputs");
    float velocity = movementSpeed * deltaTime;
    if (direction == "FORWARD")
        cameraPos += cameraFront * velocity;
    else if (direction == "BACKWARD")
        cameraPos -= cameraFront * velocity;
    else if (direction == "LEFT")
        cameraPos -= cameraRight * velocity;
    else if (direction == "RIGHT")
        cameraPos += cameraRight * velocity;
    else if (direction == "UP")
        cameraPos += cameraUp * velocity;
    else if (direction == "DOWN")
        cameraPos -= cameraUp * velocity;
    updateCameraVectors();
    
}
void Camera::processMouseMovement(float xoffset, float yoffset) {
    spdlog::debug("Processing mouse movements, xoffset:{}, yoffset:{}", xoffset, yoffset);
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Constraint pitch 
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    spdlog::debug("Processing mouse movements, yaw:{}, pitch:{}", yaw, pitch);


    updateCameraVectors();
}


}