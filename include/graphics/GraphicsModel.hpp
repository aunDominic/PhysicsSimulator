#pragma once

#include <GL/glew.h> 
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace aun{

class GraphicsModel {
protected:
    GLuint VAO, VBO, EBO;
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    GLuint *shaderProgram;

public:
    GraphicsModel();
    ~GraphicsModel();

    void initBuffers();
    void render(const glm::mat4& transform);
    void setShaderProgram(GLuint *shaderProgram);
};
}