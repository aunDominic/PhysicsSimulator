#pragma once

#include <GL/glew.h> 
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace aun{

class GraphicsModel {
protected:
    GLuint VAO, VBO, EBO;
    std::vector<GLfloat> vertices; // Expects vertices followed by its normals.
    std::vector<GLuint> indices;
    GLuint *shaderProgram;

public:
    GraphicsModel();
    ~GraphicsModel();

    void initBuffers();
    void render(const glm::mat4& transform, GLenum mode = GL_TRIANGLES);
    void setShaderProgram(GLuint *shaderProgram);
};
}