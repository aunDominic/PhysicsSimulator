#pragma once

#include <glm/glm.hpp>
#include <graphics/GraphicsModel.hpp>
#include <physics/AABB.hpp>


// The idea is that this class deals with the geometry of rigid bodies.
// Geometry involves creating a list of vertices for this Geometry 
// checking for collisions with other Geometrys
// 
namespace aun {
enum class GeometryType { Sphere, Box, Convex, Mesh };

class Geometry {
protected:
    AABB aabb;
    void updateAABB(const std::vector<glm::vec3>& points) {
        aabb = AABB::fromPoints(points);
    }
    glm::vec3 center;

public:
    GraphicsModel *graphicsModel;
    // Position and orientation
    glm::vec3 *position;                 // Center of mass
    glm::quat *orientation;              
    std::vector<glm::vec3> vertices; 

    
    Geometry(){}
    Geometry(glm::vec3 center);
    virtual ~Geometry() = default;

    virtual GeometryType getType() const = 0;
    virtual AABB getAABB() const;
    virtual void updateAABB() = 0;
    virtual glm::vec3 getSupport(const glm::vec3& direction) const = 0;
    virtual glm::vec3 getClosestPoint(const glm::vec3& point) const = 0;
    virtual float getVolume() const = 0;
    virtual float getCrossSectionArea() const = 0;
    virtual glm::mat3 getInertiaTensor(float mass) const = 0;
    virtual void transform(const glm::mat4& transformation) = 0;
     virtual std::vector<glm::vec3> getFaceNormals() const = 0;
    virtual std::vector<glm::vec3> getEdges() const = 0;
    virtual std::vector<glm::vec3> getVertices() const = 0;
    virtual std::vector<GLfloat> getFaceNormalVertices(float normalLength) const = 0;
    glm::vec3 getCenter() const { return getTransformMatrix() * glm::vec4(center, 1.0f);};
    glm::mat4 getTransformMatrix() const;
    std::vector<glm::vec3> vec3_to_world(std::vector<glm::vec3> local, bool isTranslate = 1) const;

};

}