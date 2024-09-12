#include <graphics/SeparatingPlaneModel.hpp>
#include <iostream>
#include <graphics/utils.hpp>
namespace aun{
    SeparatingPlaneModel::SeparatingPlaneModel() : GraphicsModel() {}

    void SeparatingPlaneModel::updatePlane(const CollisionInfo& collisionInfo, const Geometry *a, const Geometry *b) {
        
        // PRE: There is no collisions between a and b
        spdlog::debug("Updating plane... with normal:{}", vec3_to_string(collisionInfo.normal, 12));
        glm::vec3 normal = glm::normalize(collisionInfo.normal);
        glm::vec3 supportA = a->getSupport(normal);
        glm::vec3 supportB = b->getSupport(-normal);
        spdlog::debug("Support points: a:{}, b:{}", vec3_to_string(supportA), vec3_to_string(supportB));

        // Calculate the vector between support points
        glm::vec3 separationVector = supportB - supportA;
        
        // Project separation vector onto normal to get the true separation distance
        float separationDistance = glm::dot(separationVector, normal);
        
        // Calculate the center of the plane
        glm::vec3 center = supportA + normal * (separationDistance * 0.5f);
        
        
        // Create an orthonormal basis with the normal
        glm::vec3 tangent1 = glm::cross(normal, glm::vec3(0, 0, 1));
        if (glm::length(tangent1) < 0.01f) {
            tangent1 = glm::cross(normal, glm::vec3(1, 0, 0));
        }
        tangent1 = glm::normalize(tangent1);
        glm::vec3 tangent2 = glm::normalize(glm::cross(normal, tangent1));

        spdlog::debug("Tangents: {}, {}", vec3_to_string(tangent1), vec3_to_string(tangent2));

        // Create a square plane
        float size = 10.0f; // Adjust this to change the size of the plane
        glm::vec3 v1 = center - tangent1 * size - tangent2 * size;
        glm::vec3 v2 = center + tangent1 * size - tangent2 * size;
        glm::vec3 v3 = center + tangent1 * size + tangent2 * size;
        glm::vec3 v4 = center - tangent1 * size + tangent2 * size;

        // Update vertices (position + normal for each vertex)
        vertices = {
            v1.x, v1.y, v1.z, normal.x, normal.y, normal.z,
            v2.x, v2.y, v2.z, normal.x, normal.y, normal.z,
            v3.x, v3.y, v3.z, normal.x, normal.y, normal.z,
            v4.x, v4.y, v4.z, normal.x, normal.y, normal.z
        };

        // Update indices for two triangles
        indices = {0, 1, 2, 0, 2, 3};
        // Reinitialize buffers with new data
        spdlog::debug("Initialising plane buffers");
        initBuffers();
    }

}