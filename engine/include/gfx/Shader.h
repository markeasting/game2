#pragma once

#include "common/gl.h"

#include <string>
#include <unordered_map>
#include <cassert>
#include <vector>

class Shader {
public:

    GLuint m_program = 0;

    Shader(); 
    Shader(const std::string& vertexShader, const std::string& fragmentShader);
    Shader(const std::string& shaderName);
    ~Shader();

    /**
     * @brief Initializes the shader program by compiling and  
     * linking the vertex and fragment shaders.
     */
    void initialize();

    /**
     * @brief Gets the location of a uniform variable in the shader program.
     * @param name The name of the uniform variable. 
     */
    GLint getUniformLocation(const std::string& name);
    
    const void bind() const;
    const void unBind() const;

    static void refreshAll();

private:

    // static std::unordered_map<std::string, GLint> m_shaderCache;
    static std::vector<Shader*> m_programCache; // glGetProgramBinary()

    static constexpr const char* shaderBasePath = "assets/shader/";

    std::string m_vertexShaderPath;
    std::string m_fragmentShaderPath;

    std::unordered_map<std::string, int> uniformLocationCache = {};

    /** 
     * @brief Compiles a shader from the given source files.
     * @param shaderSource The path to the shader source file.
     * @param type The shader type for glCreateShader(): GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, etc.
     * @return The shader ID if compilation was successful, 0 otherwise.
     */
    static GLuint compile(
        const std::string& shaderSource,
        GLenum type
    );

    /**
     * @brief Creates a shader program by linking the vertex and fragment shaders.
     * @return The shader program ID if linking was successful, 0 otherwise.
     */
    static GLuint createProgram(
        const std::string& vertexShader, 
        const std::string& fragmentShader
    );
};
