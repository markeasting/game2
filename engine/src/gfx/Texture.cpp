
// #include "common/log.h"
#include "common/gl.h"

#include "gfx/Texture.h"
#include "common/gl.h"

#include <cassert>
#include <cstdio>
#include <stdexcept>
#include <string>
#include "util/Filesystem.h"

Texture::Texture() {
    create();
}

void Texture::create() {

    assert(m_texture == 0); // Ensure texture is not already created

    // if (m_texture != 0) {
    //     invalidate();
    // }

    // @todo set a default texture by abusing glBindTexture(GL_TEXTURE_2D, 0) ?
    glGenTextures(1, &m_texture);
}

void Texture::update(GLsizei width, GLsizei height, GLenum format, void* data) {

    assert(m_texture != 0);
    assert(width == m_width);
    assert(height == m_height);
    assert(format == m_format);

    // if (width != m_width || height != m_height || format != m_format) {
    //     throw std::runtime_error("Texture::update: cannot update texture data with different dimensions or format");
    // }

    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0,
        0,
        m_width,
        m_height,
        m_format,
        GL_UNSIGNED_BYTE,
        data
    );

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::load(GLsizei width, GLsizei height, GLenum format, void* data) {

    if (m_texture != 0) {
        invalidate();
        create();
    }

    m_format = format;
    m_width = width;
    m_height = height;

    glBindTexture(GL_TEXTURE_2D, m_texture);

    /* Upload data */
    glTexImage2D(
        GL_TEXTURE_2D, 
        0, 
        m_format, 
        width, 
        height, 
        0, 
        m_format, 
        GL_UNSIGNED_BYTE, 
        data
    );

    /* Generate mips */
    glGenerateMipmap(GL_TEXTURE_2D);

    /* Texture wrapping */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    /* Texture filtering: use mipmaps */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* Anisotropic filtering */
    GLfloat max_anisotropy;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &max_anisotropy);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, std::min(max_anisotropy, 16.0f));

    printf("[Texture] Initialized #%u, size: %dx%d, format: %s\n", 
        m_texture, 
        m_width, 
        m_height, 
        (m_format == GL_RGBA) ? "GL_RGBA" : "GL_RGB"
    );

    /* Unbind after completing tasks */
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::load(const char* source) {

    auto &fs = Filesystem::instance();

    stbi_set_flip_vertically_on_load_thread(1);
    
    int width, height, nrChannels;
    unsigned char *data = stbi_load(
        fs.resolveRelativePath(source).c_str(),
        &width,
        &height,
        &nrChannels,
        0
    ); 

    if (!data) {
        data = stbi_load(
            fs.resolveRelativePath(m_defaultTexture).c_str(),
            &width,
            &height,
            &nrChannels,
            0
        ); 
        printf("[Texture] Failed to load texture: %s, using default texture: %s\n", source, m_defaultTexture);
    }

    if (!data) {
        throw std::runtime_error(
            std::string("[Texture] failed to load texture: ") + source
        );
    }

    auto format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

    load(width, height, format, data);

    stbi_image_free(data); // Free the image data since it lives on the GPU now. 
    
    printf("[Texture] Loaded #%u, %s\n", m_texture, source);
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture::invalidate() {
    if (m_texture != 0) {

        printf("[Texture] Unloading #%u\n", m_texture);

        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &m_texture);

        m_texture = 0;
        m_width = 0;
        m_height = 0;
        m_format = GL_RGB; // Reset to default format
    }
}

Texture::~Texture() {
    // invalidate(); // Just kill the OpenGL context
}
