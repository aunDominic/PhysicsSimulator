#include <graphics/GraphicsModel.hpp>
#include <spdlog/spdlog.h>
#include <graphics/utils.hpp>
namespace aun{
void GraphicsModel::setShaderProgram(GLuint *shaderProgram){
    this->shaderProgram = shaderProgram;
}
GraphicsModel::GraphicsModel() {
    // initBuffers();
    // Initialize shaderProgram here
}

GraphicsModel::~GraphicsModel() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void GraphicsModel::initBuffers() {
    spdlog::debug("Initialising OpenGL buffers");

    spdlog::debug("Generating Vertex Arrays...");
    glGenVertexArrays(1, &VAO);
    spdlog::debug("Generating Vertex Buffers...");
    glGenBuffers(1, &VBO);
    spdlog::debug("Generating Element Buffers...");
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    spdlog::debug("Uploading vertex data...");
    for (auto x: vertices){
        std::cout << x << " ";
    }
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    spdlog::debug("Uploading index data...");
    for (auto x: indices){
        std::cout << x << " ";
    }
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // Set up vertex attributes (position, normal, etc.)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void GraphicsModel::render(const glm::mat4& transform) {
    glUseProgram(*shaderProgram);
    
    // Set the model matrix uniform
    GLuint modelLoc = glGetUniformLocation(*shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
}