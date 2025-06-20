
// #include "common/log.h"
#include "common/gl.h"

#include "gfx/Texture.h"
#include "common/gl.h"

#include <cstdio>
#include <stdexcept>
#include <string>
#include "util/Filesystem.h"

Texture::Texture() {}
Texture::Texture(const char* source) {
    load(source);
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
        // @todo set default texture using glBindTexture(GL_TEXTURE_2D, 0) ?
        data = stbi_load(
            fs.resolveRelativePath(m_defaultTexture).c_str(),
            &width,
            &height,
            &nrChannels,
            0
        ); 
        printf("[Texture] failed to load texture: %s, using default texture: %s\n", source, m_defaultTexture);
    }

    if (!data) {
        throw std::runtime_error(
            std::string("[Texture] failed to load texture: ") + source
        );
    }

    /* Initialize texture */
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    /* Upload data and generate mips */
    m_format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, m_format, width, height, 0, m_format, GL_UNSIGNED_BYTE, data);
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

    stbi_image_free(data);
    
    printf("[Texture] loaded texture: %s, size: %dx%d\n", source, width, height);

}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, m_texture);
}
