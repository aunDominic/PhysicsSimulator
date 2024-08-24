#pragma once
#include <physics/Geometry.hpp>
namespace aun{
class Sphere : public Geometry {
    float radius;
    int sector;
    int stack;
    GeometryType getType() const override;
    AABB getAABB() const override;
    glm::vec3 getSupport(const glm::vec3& direction) const override; // To be used in second phase of collisions detection
    glm::vec3 getClosestPoint(const glm::vec3& point) const override;
    float getVolume() const override;
    glm::mat3 getInertiaTensor(float mass) const override; // TODO: Implement getInertiaTensor
    void transform(const glm::mat4& transformation) override;

// Optional method for Geometrys that need vertex representation
    std::vector<glm::vec3> getVertices() const override;
};
}