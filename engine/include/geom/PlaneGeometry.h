#pragma once

#include "geom/Geometry.h"

class PlaneGeometry : public Geometry {
public:

    PlaneGeometry(float size = 1.0f, bool yUp = false) {
        this->generate(size, size, yUp);
    };
    PlaneGeometry(float width, float height, bool yUp = false) {
        this->generate(width, height, yUp);
    };

private:
    /* Default to z-up */
    void generate(float width, float height, bool yUp = false) {

        float x = 0.5f * width;
        float y = 0.5f * height;
        
        std::vector<Vertex> vertices {
            yUp ? Vertex(vec3(  x, 0, y ), vec3( 0, 1, 0 ), vec2( 1, 0 )) : Vertex(vec3(  x, -y, 0 ), vec3( 0, 0, 1 ), vec2( 1, 0 )),
            yUp ? Vertex(vec3(  x, 0, -y ), vec3( 0, 1, 0 ), vec2( 1, 1 )) : Vertex(vec3(  x,  y, 0 ), vec3( 0, 0, 1 ), vec2( 1, 1 )),
            yUp ? Vertex(vec3( -x, 0, y ), vec3( 0, 1, 0 ), vec2( 0, 0 )) : Vertex(vec3( -x, -y, 0 ), vec3( 0, 0, 1 ), vec2( 0, 0 )),
            yUp ? Vertex(vec3( -x, 0, -y ), vec3( 0, 1, 0 ), vec2( 0, 1 )) : Vertex(vec3( -x,  y, 0 ), vec3( 0, 0, 1 ), vec2( 0, 1 )),
        };

        std::vector<unsigned int> indices = {
            0, 1, 2,
            2, 1, 3,
        };

        this->setData(
            ref<VertexBuffer>(vertices), 
            ref<IndexBuffer>(indices)
        );
    };
};
