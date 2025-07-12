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
     * @param width Width of the texture.
     * @param height Height of the texture.
     * @param format Format of the texture (e.g., GL_RGB, GL_RGBA).
     * @param data Pointer to the texture data in memory.
     * @note The data should be in the format specified by the format parameter.
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

    GLuint getId() const {
        return m_texture;
    }

    GLsizei getWidth() const {
        return m_width;
    }

    GLsizei getHeight() const {
        return m_height;
    }

protected:

    /** OpenGL texture ID */
    GLuint m_texture = 0;

    /** Width of the texture */
    GLsizei m_width = 0;

    /** Height of the texture */
    GLsizei m_height = 0;
    
    /** Format of the texture (e.g., GL_RGB, GL_RGBA) */
    GLenum m_format = GL_RGB;

    /** 
     * Default texture path, used when a texture cannot be loaded.
     */
    static constexpr const char* m_defaultTexture = "assets/texture/default.jpg";

};
