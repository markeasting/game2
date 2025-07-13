
// #include "common/log.h"
#include "common/gl.h"

#include "gfx/Texture.h"
#include "common/gl.h"

#include <cassert>
#include <cstdio>
#include <stdexcept>
#include <string>
#include "util/Filesystem.h"

Texture::Texture(TextureSettings settings)
    : m_settings(settings) 
{
    glGenTextures(1, &m_texture);
    applySettings(settings);
}

Texture Texture::create(
    GLsizei width, 
    GLsizei height, 
    GLenum format,
    void* data
) {

    auto t = Texture();

    t.m_width = width;
    t.m_height = height;
    t.m_format = format;

    // t.applySettings();

    t.load(width, height, format, data);

    return t;
}

void Texture::applySettings(TextureSettings settings) {
    
    m_settings = settings;

    /* Bind */
    glBindTexture(GL_TEXTURE_2D, m_texture);

    /* Filtering */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_settings.minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_settings.magFilter);

    /* Wrapping */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_settings.wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_settings.wrapT);

    /* Anisotropic filtering */
    GLfloat max_anisotropy;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &max_anisotropy);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, std::min(max_anisotropy, 16.0f));

    /* Unbind */
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::load(
    GLsizei width, 
    GLsizei height, 
    GLenum format, 
    void* data
) {

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

    // this->applySettings(m_settings);

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

Texture::~Texture() {
    // invalidate(); // Just kill the OpenGL context

    printf("[Texture] Unloading #%u\n", m_texture);

    // glBindTexture(GL_TEXTURE_2D, 0);
    // glDeleteTextures(1, &m_texture);
}
