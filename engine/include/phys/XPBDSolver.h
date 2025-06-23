#pragma once

#include "common/glm.h"

#include "obj/Mesh.h"

#include "phys/RigidBody.h"
#include "phys/Constraint.h"
#include "phys/ContactSet.h"
#include "phys/CollisionPair.h"
#include <functional>

namespace XPBDSolver {

    constexpr int NUM_SUB_STEPS = 15;

    inline Ref<Mesh> p1;
    inline Ref<Mesh> p2;
    inline Ref<Mesh> r1;
    inline Ref<Mesh> r2;
    inline Ref<Mesh> n;

    inline Ref<Mesh> debugArrow;

    void init();

    void update(
        const std::vector<Ref<RigidBody>>& bodies,
        const std::vector<Ref<Constraint>>& constraints,
        const float dt,
        std::function<void(float)> onSubstep = [](float) {}
    );

    std::vector<CollisionPair> collectCollisionPairs(
        const std::vector<Ref<RigidBody>>& rigidBodies, 
        const float dt
    );

    std::vector<Ref<ContactSet>> getContacts(
        const std::vector<CollisionPair>& collisions
    );

    void solvePositions(
        const std::vector<Ref<ContactSet>>& contacts,
        const float h
    );
    
    void solveVelocities(
        const std::vector<Ref<ContactSet>>& contacts,
        const float h
    );

    void _solvePenetration(Ref<ContactSet> contact, const float h);
    void _solveFriction(Ref<ContactSet> contact, const float h);

    /** 
     * Finds the Lagrange multiplier and correction vector for a pair of bodies.
     * @param body0 First body.
     * @param body1 Second body.
     * @param corr Correction vector to apply.
     * @param dlambda Compliance (m/N).
     * @param h Substep delta time (dt/numSubSteps). 
     * @param pos0 Position on body0 to apply the correction.
     * @param pos1 Position on body1 to apply the correction.
     * @return tuple containing the Lagrange multiplier and the correction vector.
     *      - float: Lagrange multiplier (λ)
     *      - vec3: Correction with Lagrange multiplier applied (Δx)
     */
    std::tuple<float, vec3> findLagrangeMultiplier(
        RigidBody* body0,
        RigidBody* body1,
        const glm::vec3& corr,
        const float compliance,
        const float h,
        const glm::vec3& pos0 = glm::vec3(0.0f),
        const glm::vec3& pos1 = glm::vec3(0.0f)
    );

    /**
     * Apply a correction to a pair of bodies.
     * @param body0 First body.
     * @param body1 Second body.
     * @param corr Correction vector to apply.
     * @param dlambda Lagrange multiplier.
     * @param pos0 Position on body0 to apply the correction.
     * @param pos1 Position on body1 to apply the correction.
     * @param velocityLevel If true, apply the correction at the velocity level.
     */
    void applyBodyPairCorrection(
        RigidBody* body0,
        RigidBody* body1,
        const glm::vec3& corr,
        const glm::vec3& pos0 = glm::vec3(0.0f),
        const glm::vec3& pos1 = glm::vec3(0.0f),
        const bool velocityLevel = false
    );

    void debugContact(Ref<ContactSet> contact);
    void setDebugVector(const vec3& vector, const vec3& position);

}
