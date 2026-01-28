#include <physics/Geometry.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <graphics/utils.hpp>
#include <spdlog/spdlog.h>

namespace aun{
AABB Geometry::getAABB() const {
    return aabb;
}
Geometry::Geometry(glm::vec3 center) : center(center){}

glm::mat4 Geometry::getTransformMatrix() const { //duplicate code from rigid body
    spdlog::debug("Called in Geometry position:{}, orientation:", vec3_to_string(*position));
    log_mat4(glm::mat4_cast(*this->orientation));
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), *position);

    // Apply rotation (assuming rigidBody.rotation is a quaternion)
    transform *= glm::mat4_cast(*this->orientation);

 
    // Apply scaling (assuming rigidBody.scale is a vec3)
    // transform = glm::scale(transform, rigidBody.scale);
    return transform;
}
std::vector<glm::vec3> Geometry::vec3_to_world(std::vector<glm::vec3> local, bool isTranslate) const{
    std::vector<glm::vec3> world;
    world.reserve(local.size());
    glm::mat4 transform = getTransformMatrix();
    auto k = 1.0f * isTranslate;
    for (glm::vec3 vec: local){
        world.push_back(glm::vec3(transform * glm::vec4(vec, k)));
    }
    return world;
}

}