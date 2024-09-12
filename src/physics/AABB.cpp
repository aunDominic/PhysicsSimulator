#include <physics/AABB.hpp>

AABB::AABB() : min(0), max(0) {}

AABB::AABB(const glm::vec3& min, const glm::vec3& max) : min(min), max(max) {}

AABB AABB::fromPoints(const std::vector<glm::vec3>& points) {
    if (points.empty()) return AABB();
    
    glm::vec3 min = points[0], max = points[0];
    for (const auto& point : points) {
        min = glm::min(min, point);
        max = glm::max(max, point);
    }
    return AABB(min, max);
}

bool AABB::intersects(const AABB& other) const {
    return (min.x <= other.max.x && max.x >= other.min.x) &&
           (min.y <= other.max.y && max.y >= other.min.y) &&
           (min.z <= other.max.z && max.z >= other.min.z);
}

bool AABB::contains(const glm::vec3& point) const {
    return (point.x >= min.x && point.x <= max.x) &&
           (point.y >= min.y && point.y <= max.y) &&
           (point.z >= min.z && point.z <= max.z);
}

void AABB::expand(const glm::vec3& point) {
    min = glm::min(min, point);
    max = glm::max(max, point);
}

void AABB::merge(const AABB& other) {
    min = glm::min(min, other.min);
    max = glm::max(max, other.max);
}

glm::vec3 AABB::getCenter() const {
    return (min + max) * 0.5f;
}

glm::vec3 AABB::getDimensions() const {
    return max - min;
}