#pragma once
#include <physics/Geometry.hpp>
namespace aun {
class Sphere : public Geometry {
private:
    float radius;
public:
    Sphere();
    Sphere(float radius, const glm::vec3& center);
    GeometryType getType() const override;
    void updateAABB() override;
    glm::vec3 getSupport(const glm::vec3& direction) const override;
    glm::vec3 getClosestPoint(const glm::vec3& point) const override;
    float getVolume() const override;
    glm::mat3 getInertiaTensor(float mass) const override;
    void transform(const glm::mat4& transformation) override;
    float getRadius() const;
    std::vector<glm::vec3> getFaceNormals() const override;
    std::vector<glm::vec3> getEdges() const override;
    std::vector<glm::vec3> getVertices() const override;
};

}