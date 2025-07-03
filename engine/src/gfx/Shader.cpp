
#include "gfx/Shader.h"
#include "util/Filesystem.h"

/* Init static members */
// std::vector<Shader*> Shader::m_programCache;

Shader::Shader() {}

Shader::Shader(
    const std::string& vertexShader, 
    const std::string& fragmentShader
    // bool autoCompile
) : m_vertexShaderPath(vertexShader), m_fragmentShaderPath(fragmentShader) {
    
    m_program = createProgram(m_vertexShaderPath, m_fragmentShaderPath);
    
    // Shader::m_programCache.push_back(this);
}

Shader::Shader(
    const std::string& shaderName
    // bool autoCompile
) : Shader(shaderName + ".vert", shaderName + ".frag") {}

Shader::~Shader() {
    glDeleteProgram(m_program);
}

GLuint Shader::createProgram(
    const std::string& vertexShader, 
    const std::string& fragmentShader
) {

    GLuint vert = compile(
        Shader::shaderBasePath + vertexShader, 
        GL_VERTEX_SHADER
    );

    GLuint frag = compile(
        Shader::shaderBasePath + fragmentShader, 
        GL_FRAGMENT_SHADER
    );

    if (vert == 0 || frag == 0) {
        return 0;
    }

    GLuint program_id = glCreateProgram();

    glAttachShader(program_id, vert);
    glAttachShader(program_id, frag);
    
    glLinkProgram(program_id);
    glValidateProgram(program_id);

    /* Clean up vert/frag partials */
    glDeleteShader(vert);
    glDeleteShader(frag);
    glDetachShader(program_id, vert);
    glDetachShader(program_id, frag);

    /* Maybe keep/store these in a cache such that they can be re-used. */
    /* Or store the entire program binary using glGetProgramBinary() */
    // m_shaderCache[m_vertexShaderPath] = vert;
    // m_shaderCache[m_fragmentShaderPath] = frag;

    /* Check if shader linked successfully */
    int program_linked = 0;
    glGetProgramiv(program_id, GL_LINK_STATUS, &program_linked);

    if (program_linked == GL_FALSE) {

        GLsizei log_length = 0;
        GLchar message[1024];
        
        glGetProgramInfoLog(program_id, 1024, &log_length, message);

        printf("[ERROR] Shader linking error: %s\n", message);

        glDeleteProgram(program_id);

        return 0;
    }

    printf("[Shader] Program %u successfully linked.\n", program_id);

    return program_id;
}

GLuint Shader::compile(
    const std::string& shaderSource, 
    GLenum type
) {

    /* @todo maybe store/cache shaders */
    Filesystem &fs = Filesystem::instance();
    std::string shader_str = fs.getFileContents(shaderSource);

    GLuint shaderId = glCreateShader(type);
    const char* src = shader_str.c_str();
    glShaderSource(shaderId, 1, &src, nullptr);
    glCompileShader(shaderId);

    int compilation_result;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compilation_result);

    if (compilation_result == GL_FALSE) {
        int length;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
        
        char* message = (char*) malloc(length * sizeof(char));
        glGetShaderInfoLog(shaderId, length, &length, message);

        printf("[Shader] ERROR: '%s': %s", shaderSource.c_str(), message);
        
        glDeleteShader(shaderId);
        
        return 0;
    }

    printf("[Shader] Compiled %s \n", shaderSource.c_str());

    return shaderId;
}


GLint Shader::getUniformLocation(const std::string& name) {

    if (uniformLocationCache.find(name) == uniformLocationCache.end()) {
        
        glUseProgram(m_program);
        
        GLint location = glGetUniformLocation(m_program, name.c_str());
        
        // std::cout << name << std::endl;
        // assert(location != -1);
        // printf("[Shader] Uniform '%s' location: %d\n", name.c_str(), location);
        
        uniformLocationCache[name] = location;
    }

    return uniformLocationCache[name];
}

const void Shader::bind() const {
    glUseProgram(m_program);
}

const void Shader::unBind() const {
    glUseProgram(0);
}

// void Shader::refresh() {
//     glUseProgram(0);
//     glBindVertexArray(0);

//     glDeleteProgram(m_program);
//     m_program = createProgram(m_fragmentShaderPath, m_vertexShaderPath);
//     uniformLocationCache.clear(); // It'll rebuild the cache as needed
    
//     printf("[Shader] Shader program refreshed.\n");
// }

// void Shader::refreshAll() {
//     glUseProgram(0);

//     for (auto shader : Shader::m_programCache) {
//         shader->refresh();
//     }
    
//     Shader::m_programCache.clear();
//     printf("[Shader] All shader programs refreshed.\n");
// }
