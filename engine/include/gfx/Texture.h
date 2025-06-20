#pragma once

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "common/gl.h"

class Texture {
public:

    Texture();
    Texture(const char* source);

    void load(const char* source);
    virtual void bind() const;

    GLuint getTexture() const {
        return m_texture;
    }

protected:

    GLuint m_texture = 0;
    GLenum m_format = GL_RGB;

    static constexpr const char* m_defaultTexture = "assets/texture/default.jpg";

};
