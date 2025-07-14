#pragma once

#include "common/ref.h"
#include "common/glm.h"

#include "component/Component.h"
#include "geom/Geometry.h"

#include "gfx/Material.h"

struct Mesh : public Component {
public: 

    Ref<Geometry> m_geometry = nullptr;
    Ref<Material> m_material = nullptr;

    Mesh();
    Mesh(const Geometry& geometry);
    Mesh(const Geometry& geometry, const Material& material);
    Mesh(Ref<Geometry> geometry, const Material& material);

    virtual ~Mesh() {
        // std::cout << "[Mesh] Destroying mesh" << std::endl;
    };

    void setMaterial(Material& material);
    void setMaterial(Ref<Material> material);
    
    virtual void bind() const;
    virtual void unbind() const;
    
};
