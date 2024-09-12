#include <physics/CollisionDetector.hpp>
#include <algorithm>
#include <limits>
#include <spdlog/spdlog.h>
#include <iostream>
#include <physics/Sphere.hpp>
#include <graphics/utils.hpp>
namespace aun {

CollisionInfo SATDetector::checkCollision(const Geometry *a, const Geometry *b) {
    // Assume a and b are convex polyhedra
    return checkPolyhedraCollision(a, b);
}

CollisionInfo SATDetector::checkPolyhedraCollision(const Geometry *a, const Geometry *b) {
    spdlog::debug("ENTERING COLLISION CHECK");
    std::vector<glm::vec3> axes = getSeparatingAxes(a, b);
    glm::vec3 dir = glm::normalize(b->getCenter() - a->getCenter());

    CollisionInfo result;
    result.hasCollision = true;
    float minPenetration = std::numeric_limits<float>::infinity();
    const float EPSILON = 1e-9f; // Epsilon for floating point errors

    for (const auto& axis : axes) {
        glm::vec3 adjustedAxis = (glm::dot(axis, dir) >= 0) ? axis : -axis;

        float minA, maxA, minB, maxB;
        projectOntoAxis(a, axis, minA, maxA);
        projectOntoAxis(b, axis, minB, maxB);

        float overlap = calculateOverlap(minA, maxA, minB, maxB);
        spdlog::debug("Overlap: {}", overlap);
        if (overlap < EPSILON) {
            // Early exit: We found a separating axis, so there's no collision
            spdlog::debug("NO COLLISIONS FOUND at this axis:{}", vec3_to_string(adjustedAxis));
            
            result.hasCollision = false;
            result.normal = glm::normalize(adjustedAxis);
            return result;
        } else if (overlap < minPenetration) {
            spdlog::debug("1 Axis that overlaps {}, axis:{}", overlap, vec3_to_string(adjustedAxis, 12));
            minPenetration = overlap;
            result.normal = adjustedAxis;
        }
    }
    // If we get here, there's a collision
    result.penetrationDepth = minPenetration;
    // Ensure the normal points from A to B
    if (glm::dot(result.normal, b->getCenter() - a->getCenter()) < 0) {
        result.normal = -result.normal;
    }
    spdlog::debug("Collision found with depth:{}", result.penetrationDepth);
    return result;
}
std::vector<glm::vec3> SATDetector::getSeparatingAxes(const Geometry *a, const Geometry *b) {

    std::vector<glm::vec3> axes;
    glm::vec3 dir = glm::normalize(b->getCenter() - a->getCenter());

    // Add face normals from both polyhedra (assume they're normalized)
    axes = a->vec3_to_world(a->getFaceNormals(), false);
    spdlog::debug("A's face normals:");
    std::string s = "";
    for (auto vec: axes){
        s += vec3_to_string(vec) + " ";
    }
    spdlog::debug(s);
    s= "";
    auto normalsB = b->vec3_to_world(b->getFaceNormals(), false);
    spdlog::debug("B's face normals:");
    for (auto vec: normalsB){
        s += vec3_to_string(vec) + " ";
    }
    spdlog::debug(s);


    axes.insert(axes.end(), normalsB.begin(), normalsB.end());

    // Add cross products of edges
    std::vector<glm::vec3> edgesA = a->vec3_to_world(a->getEdges());
    std::vector<glm::vec3> edgesB =b->vec3_to_world(b->getEdges());
    for (const auto& edgeA : edgesA) {
        for (const auto& edgeB : edgesB) {

            glm::vec3 crossAxis = glm::normalize(glm::cross(edgeA, edgeB));
            // Filter out near-zero axes
            // and flip axes that point B to A
            if (glm::length(crossAxis) > 1e-10f) {  
                if (glm::dot(crossAxis, dir) >= 0) {
                    axes.push_back(crossAxis);
                } else {
                    axes.push_back(-crossAxis);
                }
            }
        }
    }

    return axes;
}

void SATDetector::projectOntoAxis(const Geometry *polyhedron, const glm::vec3& axis, float& min, float& max) {
    spdlog::debug("PROJECTION of vertices onto axis: {}", vec3_to_string(axis));
    if (const Sphere* sphere = dynamic_cast<const Sphere*>(polyhedron)) {
        // Special case for sphere
        float centerProjection = glm::dot(sphere->getVertices()[0], axis);
        float radius = sphere->getRadius();
        min = centerProjection - radius;
        max = centerProjection + radius;
        return;
    }
    auto vertices = polyhedron->vec3_to_world(polyhedron->getVertices());
    min = max = glm::dot(vertices[0], axis);
    for (const auto& vertex : vertices) {
        float projection = glm::dot(vertex, axis);        
        if (projection < min) {
            min = projection;
        }
        if (projection > max) {
            max = projection;
    }
    }
}

float SATDetector::calculateOverlap(float minA, float maxA, float minB, float maxB) {
    spdlog::debug("Calculating overlap: A({}, {}) - B({}, {})", minA, maxA, minB, maxB);
    return std::min(maxA, maxB) - std::max(minA, minB);
}

}
