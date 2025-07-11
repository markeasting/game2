#pragma once

#include "common/glm.h"

struct Vertex {
    
    vec3 position = vec3(0);
    vec3 normal   = vec3(0);
    vec2 uv       = vec2(0);
    // vec4 color    = vec4(0);

    Vertex();
    
    Vertex(vec3 position = vec3(0), vec3 normal = vec3(0), vec2 uv = vec2(0));
    Vertex(float x = 0, float y = 0, float z = 0);
	
    ~Vertex() = default;

    /**
     * Sets vertex attribute pointers after creating a VAO. 
     * uses `glVertexAttribPointer()` to set the stride and offsets 
     * for the Vertex struct. 
     */
    static void setAttribPointers();

};
