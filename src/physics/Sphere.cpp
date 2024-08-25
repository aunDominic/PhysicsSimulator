#include <physics/Sphere.hpp>


namespace aun{

GeometryType Sphere::getType() const  {
    return GeometryType::Sphere; // Provide a default type
}

AABB Sphere::getAABB() const  {
    return AABB(); // Provide a default AABB
}

glm::vec3 Sphere::getSupport(const glm::vec3& direction) const  {
    return glm::vec3(0.0f); // Provide a default support vector
}

glm::vec3 Sphere::getClosestPoint(const glm::vec3& point) const  {
    return glm::vec3(0.0f); // Provide a default closest point
}

float Sphere::getVolume() const  {
    return 0.0f; // Provide a default volume
}

glm::mat3 Sphere::getInertiaTensor(float mass) const  {
    return glm::mat3(1.0f); // Provide a default inertia tensor
}

void Sphere::transform(const glm::mat4& transformation)  {
    // No-op: Default implementation does nothing
}

std::vector<glm::vec3> Sphere::getVertices() const  {
    return {}; // Provide an empty default vertex list
}

}