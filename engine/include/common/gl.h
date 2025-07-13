#pragma once

#include <GL/glew.h>

#define CHECK_GL_ERROR() \
    do { \
        GLenum error = glGetError(); \
        if (error != GL_NO_ERROR) { \
            printf("[OpenGL Error] Code: %d\n", error); \
            throw std::runtime_error("OpenGL error occurred."); \
        } \
    } while (0)
