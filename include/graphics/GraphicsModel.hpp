#pragma once

#include <GL/glew.h> 
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace aun{

class GraphicsModel {
protected:
    GLuint VAO, VBO, EBO;
    GLuint normalVAO, normalVBO;
    std::vector<GLfloat> normalVertices;
    std::vector<GLfloat> vertices; // Expects vertices followed by its normals.
    std::vector<GLuint> indices;
    std::vector<glm::vec3> corners; // Defines the vertexes of the model
    std::vector<glm::vec3> faceNormals; // Defines the normals at these faces
    std::vector<std::vector<unsigned int>> faceIndices; // Defines the indexes of each face
    GLuint *shaderProgram;

public:
    GraphicsModel();
    ~GraphicsModel();

    void initBuffers();
    void render(const glm::mat4& transform, GLenum mode = GL_TRIANGLES);
    void setShaderProgram(GLuint *shaderProgram);
    void initNormalBuffers();
    void renderNormals(const glm::mat4& transform);
    void setNormalVertices(std::vector<GLfloat> normalVertices){this->normalVertices = normalVertices;};

};
}