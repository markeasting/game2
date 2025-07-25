#pragma once

#include "geom/Geometry.h"

class ArrowGeometry : public Geometry {
public:

    ArrowGeometry(float size = 1.0f) {
        this->generate(size);
    };

private:
    void generate(float size) {

        std::vector<Vertex> vertices = { 
            Vertex(vec3(0, 0, 0)),      Vertex(vec3(0, size, 0)),
            Vertex(vec3(0, size, 0)),   Vertex(vec3(-0.2/3.0f * size, 0.8 * size, 0)),
            Vertex(vec3(0, size, 0)),   Vertex(vec3( 0.2/3.0f * size, 0.8 * size, 0))
        };

        this->setData(
            ref<VertexBuffer>(vertices)
        );
    };
};
