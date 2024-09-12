
#include <physics/BoxGeometry.hpp>
#include <graphics/BoxModel.hpp>
#include <graphics/utils.hpp>
#include <algorithm>
#include <spdlog/spdlog.h>

namespace aun {

BoxGeometry::BoxGeometry(const glm::vec3& center, float width, float height, float depth)
    : halfExtents(width / 2.0f, height / 2.0f, depth / 2.0f), Geometry(center)
{
    graphicsModel = new BoxModel(halfExtents);

}

GeometryType BoxGeometry::getType() const {
    return GeometryType::Box; // Assuming you have this enum defined
}

void BoxGeometry::updateAABB()  {
    glm::vec3 min = center - halfExtents;
    glm::vec3 max = center + halfExtents;
    aabb = AABB(min, max);
}

glm::vec3 BoxGeometry::getSupport(const glm::vec3& direction) const {
    // Get the transformation matrix
    glm::mat4 transform = getTransformMatrix();
    glm::mat4 inverseTransform = glm::inverse(transform);

    // Transform the direction to local space
    glm::vec3 localDir = glm::vec3(inverseTransform * glm::vec4(direction, 0.0f));
    // Compute the support point in local space
    glm::vec3 localSupport = glm::sign(localDir) * halfExtents;
    
    // Transform the support point back to world space
    glm::vec4 worldSupport = transform * glm::vec4(localSupport, 1.0f);
    spdlog::debug("Support point:{}", vec3_to_string(worldSupport));
    return glm::vec3(worldSupport);
}

glm::vec3 BoxGeometry::getClosestPoint(const glm::vec3& point) const  {
    return glm::clamp(point, center - halfExtents, center + halfExtents);
}

float BoxGeometry::getVolume() const  {
    return 8.0f * halfExtents.x * halfExtents.y * halfExtents.z;
}
float BoxGeometry::getCrossSectionArea() const {
    auto average = (halfExtents.x + halfExtents.y + halfExtents.z) / 3; 
    return 4.0f * average * average;
}
glm::mat3 BoxGeometry::getInertiaTensor(float mass) const  {
    float x2 = halfExtents.x * halfExtents.x;
    float y2 = halfExtents.y * halfExtents.y;
    float z2 = halfExtents.z * halfExtents.z;
    float factor = mass / 3.0f;
    return glm::mat3(
        factor * (y2 + z2), 0, 0,
        0, factor * (x2 + z2), 0,
        0, 0, factor * (x2 + y2)
    );
}

void BoxGeometry::transform(const glm::mat4& transformation)  {
    center = glm::vec3(transformation * glm::vec4(center, 1.0f));
    glm::mat3 rotationScale = glm::mat3(transformation);
    halfExtents = glm::abs(rotationScale * halfExtents);
    updateAABB();
}
std::vector<glm::vec3> BoxGeometry::getVertices() const {
    spdlog::debug("Creating local vertices...");
    return {
        glm::vec3(-halfExtents.x, -halfExtents.y, -halfExtents.z), // 0: left-bottom-front
        glm::vec3(halfExtents.x, -halfExtents.y, -halfExtents.z),  // 1: right-bottom-front
        glm::vec3(halfExtents.x, halfExtents.y, -halfExtents.z),   // 2: right-top-front
        glm::vec3(-halfExtents.x, halfExtents.y, -halfExtents.z),  // 3: left-top-front
        glm::vec3(-halfExtents.x, -halfExtents.y, halfExtents.z),  // 4: left-bottom-back
        glm::vec3(halfExtents.x, -halfExtents.y, halfExtents.z),   // 5: right-bottom-back
        glm::vec3(halfExtents.x, halfExtents.y, halfExtents.z),    // 6: right-top-back
        glm::vec3(-halfExtents.x, halfExtents.y, halfExtents.z)    // 7: left-top-back
    };

}

std::vector<glm::vec3> BoxGeometry::getFaceNormals() const {
    spdlog::debug("Creating Face Normals...");
   
    return {
         glm::vec3(0, 0, -1),  // Front
         glm::vec3(0, 0, 1),    // Back
         glm::vec3(0, -1, 0),  // Bottom
         glm::vec3(0, 1, 0),    // Top
         glm::vec3(-1, 0, 0),  // Left
         glm::vec3(1, 0, 0)    // Right
    };
}
std::vector<glm::vec3> BoxGeometry::getEdges() const {
    // Vertices of the box
    std::vector<glm::vec3> vertices = getVertices(); 
    // List edges as vectors (start to end)
    return {
        vertices[1] - vertices[0], vertices[2] - vertices[1], vertices[3] - vertices[2], vertices[0] - vertices[3], // Bottom edges
        vertices[5] - vertices[4], vertices[6] - vertices[5], vertices[7] - vertices[6], vertices[4] - vertices[7], // Top edges
        vertices[0] - vertices[4], vertices[1] - vertices[5], vertices[2] - vertices[6], vertices[3] - vertices[7]  // Vertical edges
    };
}

std::vector<GLfloat> BoxGeometry::getFaceNormalVertices(float normalLength) const {
    spdlog::debug("Creating normal vertices...");
    std::vector<GLfloat> normalVertices;
    std::vector<glm::vec3> faceNormals = getFaceNormals();
    spdlog::debug("Face normals to be drawn:");
    for (auto x: faceNormals){
       spdlog::debug(vec3_to_string(x));
    }

    std::vector<glm::vec3> vertices = getVertices();

    // For each face, we'll use its center as the starting point of the normal line
    std::vector<glm::vec3> faceCenters = {
        (vertices[0] + vertices[1] + vertices[2] + vertices[3]) / 4.0f,  // Front face
        (vertices[4] + vertices[5] + vertices[6] + vertices[7]) / 4.0f,  // Back face
        (vertices[0] + vertices[1] + vertices[4] + vertices[5]) / 4.0f,  // Bottom face
        (vertices[2] + vertices[3] + vertices[6] + vertices[7]) / 4.0f,  // Top face
        (vertices[0] + vertices[3] + vertices[4] + vertices[7]) / 4.0f,  // Left face
        (vertices[1] + vertices[2] + vertices[5] + vertices[6]) / 4.0f   // Right face
    };
    std::string s = "";

    for (size_t i = 0; i < faceNormals.size(); ++i) {
        glm::vec3 start = faceCenters[i];
        glm::vec3 end = start + faceNormals[i] * normalLength;
        s += vec3_to_string(start) + " " + vec3_to_string(end) + "\n";

        // Start point
        normalVertices.push_back(start.x);
        normalVertices.push_back(start.y);
        normalVertices.push_back(start.z);

        // End point
        normalVertices.push_back(end.x);
        normalVertices.push_back(end.y);
        normalVertices.push_back(end.z);
    }

    spdlog::debug("Returning normal vertices.");
    spdlog::debug(s);

    return normalVertices;
}

}