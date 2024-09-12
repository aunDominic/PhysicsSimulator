// Sphere.cpp

#include <physics/Sphere.hpp>
#include <graphics/SphereModel.hpp>
#include <spdlog/spdlog.h>
#include <glm/gtc/constants.hpp>
#include <graphics/utils.hpp>

namespace aun {

Sphere::Sphere() : radius(1.0f), Geometry(glm::vec3(0.0f)) {
    graphicsModel = new SphereModel();

    updateAABB();
}

Sphere::Sphere(float radius, const glm::vec3& center)
    : radius(radius), Geometry(center) {
    graphicsModel = new SphereModel();

    updateAABB();
}

GeometryType Sphere::getType() const {
    return GeometryType::Sphere;
}

void Sphere::updateAABB() {
    glm::vec3 radiusVec(radius);
    aabb = AABB(center - radiusVec, center + radiusVec);
}

glm::vec3 Sphere::getSupport(const glm::vec3& direction) const {
    return center + glm::normalize(direction) * radius;
}

glm::vec3 Sphere::getClosestPoint(const glm::vec3& point) const {
    glm::vec3 dir = point - center;
    float distance = glm::length(dir);
    if (distance <= radius) {
        return point;
    }
    return center + (dir / distance) * radius;
}

float Sphere::getVolume() const {
    return (4.0f / 3.0f) * glm::pi<float>() * radius * radius * radius;
}
float Sphere::getCrossSectionArea() const {
    return radius * radius;
}

glm::mat3 Sphere::getInertiaTensor(float mass) const {
    float i = 2.0f * mass * radius * radius / 5.0f;
    return glm::mat3(
        i, 0, 0,
        0, i, 0,
        0, 0, i
    );
}

void Sphere::transform(const glm::mat4& transformation) {
    center = glm::vec3(transformation * glm::vec4(center, 1.0f));
    updateAABB();
}

float Sphere::getRadius() const { return radius; }

std::vector<glm::vec3> Sphere::getFaceNormals() const {
    return {}; // Spheres don't have flat faces
}

std::vector<glm::vec3> Sphere::getEdges() const {
    return {}; // Spheres don't have edges
}

std::vector<glm::vec3> Sphere::getVertices() const {
    spdlog::debug("TRANSFORM MATRIX FOR vertices");
    log_mat4(getTransformMatrix());
    return {getTransformMatrix() * glm::vec4(center, 1.0f)}; // The sphere can be represented by its center for many calculations
}
std::vector<GLfloat> Sphere::getFaceNormalVertices(float normalLength) const {
    
}

} // namespace aun