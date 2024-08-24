#include <graphics/utils.hpp>
#include <spdlog/spdlog.h>
namespace aun{

std::string loadShaderSource(const char* filePath) {
    std::ifstream shaderFile(filePath);
    if (!shaderFile.is_open()) {
        spdlog::error("Failed to open shader file: {}", filePath);
        return "";
    }
    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();
    return shaderStream.str();
}
GLuint createShader(const char* source, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    
    // Check for compilation errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Error: Shader compilation failed\n" << infoLog << std::endl;
    }
    
    return shader;
}

GLuint createShaderProgram(const char* vertexSourcePath, const char* fragmentSourcePath) {
    std::string vertexCode = loadShaderSource(vertexSourcePath);
    std::string fragmentCode = loadShaderSource(fragmentSourcePath);

    if (vertexCode.empty() || fragmentCode.empty()) {
        spdlog::error("Shader source code could not be loaded.");
        return 0;
    }
    GLuint vertexShader = createShader(vertexCode.c_str(), GL_VERTEX_SHADER);
    GLuint fragmentShader = createShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);
    
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // Check for linking errors
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        spdlog::error("Error: Shader program linking failed{}\n",infoLog);
    }
    
    // Clean up shaders (they are linked into the program and no longer needed separately)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}
void log_mat4(const glm::mat4& mat) {
    std::ostringstream oss;
    oss << "glm::mat4:\n";
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            oss << mat[i][j] << " ";
        }
        oss << "\n";
    }
    spdlog::info(oss.str());
}
}