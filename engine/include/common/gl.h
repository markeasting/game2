#pragma once

#include <GL/glew.h>

#define CHECK_GL_ERROR() \
    do { \
        GLenum error = glGetError(); \
        if (error != GL_NO_ERROR) { \
            printf("[OpenGL ERR] Code: %d, File: %s:%d\n", \
                error, __FILE__, __LINE__); \
        } \
    } while (0)

#define CHECK_GL_ERROR_THROW() \
    do { \
        GLenum error = glGetError(); \
        if (error != GL_NO_ERROR) { \
            throw std::runtime_error( \
                std::string("[OpenGL ERR] Code: ") + std::to_string(error) + \
                ", File: " + __FILE__ + ":" + std::to_string(__LINE__) \
            ); \
        } \
    } while (0)

#define GL_CLEAR_ERRORS() \
    do { \
        while (glGetError() != GL_NO_ERROR); \
    } while (0)
