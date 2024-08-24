#pragma once
#include <GL/glew.h>
#include <iostream>
#include <glm/glm.hpp>
#include <sstream>
#include <fstream>


namespace aun{
GLuint createShader(const char* source, GLenum shaderType);
GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource);
void log_mat4(const glm::mat4& mat);
std::string loadShaderSource(const char* filePath);
}