#pragma once

#include "phys/RigidBody.h"
#include "phys/Constraint.h"
#include "obj/Mesh.h"

#include <vector>
#include <functional>

struct RaycastInfo {
    bool exists = false;
    vec3 point = vec3();
    vec3 normal = vec3();
    float dist = 0.0f;
};

class PhysicsHandler {
public:

	std::vector<Ref<RigidBody>> m_bodies = {};
	std::vector<Ref<Constraint>> m_constraints = {};
    std::vector<Ref<Mesh>> m_debugMeshes;

    PhysicsHandler() = default;
    ~PhysicsHandler() = default;

    void add(Ref<RigidBody> body);

    void init();
    
    void update(float dt);

    /**
     * Update method that allows for custom update logic.
     * @param dt Delta time since last update.
     * @param onSubstep Callback that receives a substep interval (h)
     */
    void update(float dt, std::function<void(float)> onSubstep);

    RaycastInfo raycast(const vec3& ray_origin, const vec3& ray_dir) const;

};
