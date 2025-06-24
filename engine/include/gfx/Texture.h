#pragma once

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "common/gl.h"

class Texture {
public:

    Texture();
    ~Texture();

    /**
     * @brief Initializes the texture object using glGenTextures().
     */
    void create();

    /** 
     * @brief Load texture from a file.
     */
    void load(const char* source);

    /** 
     * @brief Load texture data from a file or memory. 
     * @see glGenTextures / glTexImage2D
     */
    void load(GLsizei width, GLsizei height, GLenum format, void* data);

    /**
     * @brief Updates the texture with new data.
     * @param data Pointer to the new texture data.
     */
    void update(GLsizei width, GLsizei height, GLenum format, void* data);

    /**
     * @brief Releases the texture resources.
     */
    void invalidate();

    /**
     * @brief Binds the texture to the current OpenGL context.
     * @see glBindTexture
     */
    virtual void bind() const;

    GLuint getTexture() const {
        return m_texture;
    }

protected:

    GLuint m_texture = 0;
    GLsizei m_width = 0;
    GLsizei m_height = 0;
    
    GLenum m_format = GL_RGB;

    static constexpr const char* m_defaultTexture = "assets/texture/default.jpg";

};
