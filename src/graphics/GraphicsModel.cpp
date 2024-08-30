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
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        spdlog::error("OpenGL Error: {}", error);
    }

    spdlog::debug("Generating Vertex Buffers...");
    glGenBuffers(1, &VBO);
    spdlog::debug("Generating Element Buffers...");
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    spdlog::debug("Generated VAO ID: {}", VAO);

    spdlog::debug("Uploading vertex data... length:{}", vertices.size());

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    spdlog::debug("Uploading index data... length:{}", indices.size());
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
    spdlog::debug("Uploaded index data. length:{}", indices.size());

    // Set up vertex attributes (position, normal, etc.)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    spdlog::debug("Unbinding vertex array and buffer data");

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void GraphicsModel::render(const glm::mat4& transform, GLenum mode) {
    glUseProgram(*shaderProgram);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        spdlog::error("OpenGL error on enter: {}", err);
    }

    // Set the model matrix uniform
    GLuint modelLoc = glGetUniformLocation(*shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));
   
    // Bind the VAO
    err = glGetError();

    glBindVertexArray(VAO);
    err = glGetError();
    if (err != GL_NO_ERROR) {
        spdlog::error("OpenGL error on binding VAO: {}", err);
    }

    // Draw the elements
    glLineWidth(0.5f);
    glDrawElements(mode, indices.size(), GL_UNSIGNED_INT, 0);
    
    // Unbind the VAO
    glBindVertexArray(0);
}

}