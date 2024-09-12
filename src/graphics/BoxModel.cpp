#include <graphics/BoxModel.hpp>
namespace aun{

void BoxModel::createBox() {
    // Clear any existing data
    vertices.clear();
    indices.clear();

    // Generate vertices
    for (int i = 0; i < 8; ++i) {
        // Position
        vertices.push_back(corners[i].x);
        vertices.push_back(corners[i].y);
        vertices.push_back(corners[i].z);

        // Normal (to be calculated later)
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
    }

    // Generate indices
    unsigned int cubeIndices[] = {
        0, 1, 2, 2, 3, 0, // Front face
        4, 5, 6, 6, 7, 4, // Back face
        0, 4, 5, 5, 1, 0, // Bottom face
        3, 2, 6, 6, 7, 3, // Top face
        0, 3, 7, 7, 4, 0, // Left face
        1, 5, 6, 6, 2, 1  // Right face
    };
    indices.insert(indices.end(), std::begin(cubeIndices), std::end(cubeIndices));

    // Calculate vertex normals by taking the average 
    std::vector<glm::vec3> vertexNormals(8, glm::vec3(0.0f));
    for (int i = 0; i < 6; ++i) {
        glm::vec3 v0 = corners[faceIndices[i][0]];
        glm::vec3 v1 = corners[faceIndices[i][1]];
        glm::vec3 v2 = corners[faceIndices[i][2]];
        glm::vec3 faceNormal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
        
        for (int j = 0; j < 4; ++j) {
            vertexNormals[faceIndices[i][j]] += faceNormal;
        }
    }

    // Normalize and set the vertex normals
    for (int i = 0; i < 8; ++i) {
        glm::vec3 normal = glm::normalize(vertexNormals[i]);
        vertices[i * 6 + 3] = normal.x;
        vertices[i * 6 + 4] = normal.y;
        vertices[i * 6 + 5] = normal.z;
    }
}
BoxModel::BoxModel(glm::vec3 halfExtents) : halfExtents(halfExtents){
    corners = {
        {-halfExtents.x, -halfExtents.y, -halfExtents.z}, // 0: Front bottom left
        { halfExtents.x, -halfExtents.y, -halfExtents.z}, // 1: Front bottom right
        { halfExtents.x,  halfExtents.y, -halfExtents.z}, // 2: Front top right
        {-halfExtents.x,  halfExtents.y, -halfExtents.z}, // 3: Front top left
        {-halfExtents.x, -halfExtents.y,  halfExtents.z}, // 4: Back bottom left
        { halfExtents.x, -halfExtents.y,  halfExtents.z}, // 5: Back bottom right
        { halfExtents.x,  halfExtents.y,  halfExtents.z}, // 6: Back top right
        {-halfExtents.x,  halfExtents.y,  halfExtents.z}  // 7: Back top left
    };
    createBox();
    initBuffers();
}
}