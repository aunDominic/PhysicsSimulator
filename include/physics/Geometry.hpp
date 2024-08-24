#pragma once

#include <glm/glm.hpp>
#include <graphics/GraphicsModel.hpp>

// The idea is that this class deals with the geometry of rigid bodies.
// Geometry involves creating a list of vertices for this Geometry 
// checking for collisions with other Geometrys
// 
namespace aun {
enum class GeometryType { Sphere, Box, Convex, Mesh };

class AABB {
    glm::vec3 min, max;
public:
    // AABB methods 
    // TODO: Implement AABB methods for broad phase collisions
};

class Geometry {
public:
    std::unique_ptr<GraphicsModel> graphicsModel;
    virtual ~Geometry() = default;
    virtual GeometryType getType() const = 0;
    virtual AABB getAABB() const = 0;
    virtual glm::vec3 getSupport(const glm::vec3& direction) const = 0; // To be used in second phase of collisions detection
    virtual glm::vec3 getClosestPoint(const glm::vec3& point) const = 0;
    virtual float getVolume() const = 0;
    virtual glm::mat3 getInertiaTensor(float mass) const = 0; // TODO: Implement getInertiaTensor
    virtual void transform(const glm::mat4& transformation) = 0;

    // Optional method for Geometrys that need vertex representation
    virtual std::vector<glm::vec3> getVertices() const { return {}; }
 
};



// class BoxGeometry : public Geometry {
// public:
//     glm::vec3 halfExtents;

// };

// Similar classes for Capsule and Cylinder
}