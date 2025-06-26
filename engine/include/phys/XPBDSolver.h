#pragma once

#include "common/glm.h"

#include "component/Mesh.h"

#include "phys/RigidBody.h"
#include "phys/Constraint.h"
#include "phys/ContactSet.h"
#include "phys/CollisionPair.h"
#include <functional>

namespace XPBDSolver {

    constexpr int NUM_SUB_STEPS = 15;

    void init();

    /**
     * @brief Updates the physics simulation for a given time step.
     * @param bodies Vector of rigid bodies to update.
     * @param constraints Vector of constraints to apply.
     * @param dt Delta time for the current physics step.
     * @param onSubstep Callback function to execute after each substep.
     */
    void update(
        const std::vector<Ref<RigidBody>>& bodies,
        const std::vector<Ref<Constraint>>& constraints,
        const float dt,
        std::function<void(float)> onSubstep = [](float) {}
    );

    /**
     * @brief Collects potential collision pairs (broad phase)
     * @param rigidBodies Vector of rigid bodies to check for collisions.
     * @param dt Delta time for the current physics step.
     * @return Vector of basic collision pairs.
     */
    std::vector<CollisionPair> collectCollisionPairs(
        const std::vector<Ref<RigidBody>>& rigidBodies, 
        const float dt
    );

    /**
     * @brief Gets contacts from the collected collision pairs (narrow phase).
     * @param collisions Vector of collision pairs to process.
     * @return Vector of detailed contact sets.
     */
    std::vector<Ref<ContactSet>> getContacts(
        const std::vector<CollisionPair>& collisions
    );

    /**
     * @brief Position-level solver for resolving contact and friction.
     * @param contacts Vector of contact sets to process.
     * @param h Substep delta time (dt / NUM_SUB_STEPS).
     */
    void solvePositions(
        const std::vector<Ref<ContactSet>>& contacts,
        const float h
    );
    
    /**
     * @brief Velocity-level solver for contacts.
     * @param contacts Vector of contact sets to process.
     * @param h Substep delta time (dt / NUM_SUB_STEPS).
     */
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
     * @return tuple containing the Lagrange multiplier and correction vector.
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

    /* Debugging tools ------------------------------------------------------ */
    
    void debugContact(Ref<ContactSet> contact);
    void setDebugVector(const vec3& vector, const vec3& position);

    inline Ref<Mesh> p1;
    inline Ref<Mesh> p2;
    inline Ref<Mesh> r1;
    inline Ref<Mesh> r2;
    inline Ref<Mesh> n;
    inline Ref<Mesh> debugArrow;
}
