#pragma once
#include <GL/glew.h>
#include <iostream>
#include <glm/glm.hpp>
#include <sstream>
#include <fstream>
#include <iomanip>
namespace aun{
GLuint createShader(const char* source, GLenum shaderType);
GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource);
void log_mat4(const glm::mat4& mat);
void log_mat3(const glm::mat3& mat);
std::string loadShaderSource(const char* filePath);
inline std::string vec3_to_string(const glm::vec3& v, int precision = 2);
void log_vector(const glm::vec3& v, const std::string& label= "");
}