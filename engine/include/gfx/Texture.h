#pragma once

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "common/gl.h"

struct TextureSettings {
    GLint minFilter = GL_LINEAR;
    GLint magFilter = GL_LINEAR;
    GLint wrapS = GL_REPEAT;
    GLint wrapT = GL_REPEAT;
};

class Texture {
public:

    /**
     * @brief Initializes the texture object using glGenTextures().
     */
    Texture(TextureSettings settings = {});

    ~Texture();

    /**
     * @brief Creates a texture with specified width, height, and format.
     * @param width Width of the texture.
     * @param height Height of the texture.
     * @param format Format of the texture (e.g., GL_RGB, GL_RGBA).
     * @param data Optional pointer to the texture data.
     */
    static Texture create(
        GLsizei width, 
        GLsizei height, 
        GLenum format = GL_RGB,
        void* data = nullptr
    );

    void applySettings(TextureSettings settings);

    /** 
     * @brief Load texture from a file.
     */
    void load(const char* source);

    /** 
     * @brief Load texture data from a file or memory. 
     * @see glTexImage2D()
     * @param width Width of the texture.
     * @param height Height of the texture.
     * @param format Format of the texture (e.g., GL_RGB, GL_RGBA).
     * @param data Pointer to the texture data in memory.
     * @note The data should be in the format specified by the format parameter.
     */
    void load(
        GLsizei width, 
        GLsizei height, 
        GLenum format, 
        void* data
    );

    /**
     * @brief Binds the texture to the current OpenGL context.
     * @see glBindTexture()
     * @todo Maybe add a parameter to specify which texture unit to bind to?
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

    TextureSettings m_settings;

    /** OpenGL texture ID */
    GLuint m_texture = 0;

    /** Width of the texture - determined after data is loaded */
    GLsizei m_width = 0;

    /** Height of the texture - determined after data is loaded */
    GLsizei m_height = 0;
    
    /** Format of the texture (e.g. GL_RGB, GL_RGBA) */
    GLenum m_format = GL_RGB;

    /** 
     * Default texture path, used when a texture cannot be loaded.
     */
    static constexpr const char* m_defaultTexture = "assets/texture/default.jpg";

};
