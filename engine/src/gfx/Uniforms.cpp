#include "gfx/Uniforms.h"

#include "common/glm.h"
#include "common/gl.h"

/* @TODO check if the branching of m_needsUpdate actually decreases performance. */
template<> void Uniform<int>::bind() { 
    // if (m_needsUpdate) {
        glUniform1i(m_location, m_value); 
        // m_needsUpdate = false; // causes issues with materials that share shaders
    // }
}

template<> void Uniform<float>::bind() { 
    // if (m_needsUpdate) {
        glUniform1f(m_location, m_value); 
        // m_needsUpdate = false; // causes issues with materials that share shaders
    // }
}

template<> void Uniform<vec3>::bind() { 
    // if (m_needsUpdate) {
        glUniform3f(m_location, m_value.x, m_value.y, m_value.z); 
        // m_needsUpdate = false; // causes issues with materials that share shaders
    // }
}

template<> void Uniform<vec4>::bind() { 
    // if (m_needsUpdate) {
        glUniform4f(m_location, m_value.x, m_value.y, m_value.z, m_value.w); 
        // m_needsUpdate = false; // causes issues with materials that share shaders
    // }
}

template<> void Uniform<mat4>::bind() { 
    // if (m_needsUpdate) {
        glUniformMatrix4fv(m_location, 1, GL_FALSE, value_ptr(m_value)); 
        // m_needsUpdate = false; // causes issues with materials that share shaders
    // }
}

template<> void Uniform<bool>::bind() { 
    // if (m_needsUpdate) {
        glUniform1i(m_location, m_value); 
        // m_needsUpdate = false; // causes issues with materials that share shaders
    // }
}
