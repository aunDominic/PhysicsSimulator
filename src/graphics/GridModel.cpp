#include <graphics/GridModel.hpp>

namespace aun{
GridModel::GridModel() {
    gridSize = 100;
    lineSpacing = 1.0f;
}
GridModel::GridModel(int gridSize) : gridSize(gridSize){

}


void GridModel::createGrid(){
    GLfloat normal[3] = {0.0f, 1.0f, 0.0f}; // Normals pointing up (Y-axis)
    // Clear previous data if any
    vertices.clear();
    indices.clear();
    int vertexIndex = 0;

    // Lines parallel to Z axis
    for (int i = -gridSize; i <= gridSize; i++) {
        // First vertex of the line
        vertices.push_back((float)i * lineSpacing); // x1
        vertices.push_back(0.0f);                   // y1
        vertices.push_back((float)-gridSize * lineSpacing); // z1
        vertices.push_back(normal[0]);
        vertices.push_back(normal[1]);
        vertices.push_back(normal[2]);
        indices.push_back(vertexIndex++);


        // Second vertex of the line
        vertices.push_back((float)i * lineSpacing); // x2
        vertices.push_back(0.0f);                   // y2
        vertices.push_back((float)gridSize * lineSpacing); // z2
        vertices.push_back(normal[0]);
        vertices.push_back(normal[1]);
        vertices.push_back(normal[2]);
        indices.push_back(vertexIndex++);

    }
    // Lines parallel to X axis
    for (int i = -gridSize; i <= gridSize; i++) {
        // First vertex of the line
        vertices.push_back((float)-gridSize * lineSpacing); // x1
        vertices.push_back(0.0f);                           // y1
        vertices.push_back((float)i * lineSpacing);         // z1
        vertices.push_back(normal[0]);
        vertices.push_back(normal[1]);
        vertices.push_back(normal[2]);
        indices.push_back(vertexIndex++);


        // Second vertex of the line
        vertices.push_back((float)gridSize * lineSpacing);  // x2
        vertices.push_back(0.0f);                           // y2
        vertices.push_back((float)i * lineSpacing);         // z2
        vertices.push_back(normal[0]);
        vertices.push_back(normal[1]);
        vertices.push_back(normal[2]);
        indices.push_back(vertexIndex++);

    }

}

}