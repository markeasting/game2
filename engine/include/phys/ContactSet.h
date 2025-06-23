#pragma once

#include "common/glm.h"
#include "phys/RigidBody.h"

/**
 * ContactSet represents a contact between two rigid bodies.
 * It contains information about the contact points, normal, penetration depth,
 * relative velocity, and friction properties.
 */
struct ContactSet {

    RigidBody* A = nullptr;
    RigidBody* B = nullptr;

    vec3 p1 = vec3(0.0f);
    vec3 p2 = vec3(0.0f);

    vec3 r1 = vec3(0.0f);
    vec3 r2 = vec3(0.0f);

    vec3 n = vec3(0.0f);

    float d = 0.0f;
    
    vec3 vrel = vec3(0.0f);

    float vn = 0.0f;

    float e = 0.5f;
    
    float staticFriction = 0.0f;
    float dynamicFriction = 0.0f;

    float lambda_n = 0.0f;
    float lambda_t = 0.0f;

    ContactSet(
        RigidBody* A, 
        RigidBody* B,
        vec3 normal,
        float d,
        vec3 _p1,
        vec3 _p2,
        vec3 _r1,
        vec3 _r2
    ): A(A), B(B), n(normal), d(d), p1(_p1), p2(_p2), r1(_r1), r2(_r2) {

        assert(A != B);

        /*
         * (29) Relative velocity
         * This is calculated before the velocity solver,
         * so that we can solve restitution (Eq. 34).
         * 
         * ṽn == vn
         */
        vrel = A->getVelocityAt(p1) - B->getVelocityAt(p2);
        vn = dot(n, vrel);

        /* Option 1: Multiply surface properties */
        e = A->restitution * B->restitution;
        staticFriction = A->staticFriction * B->staticFriction;
        dynamicFriction = A->dynamicFriction * B->dynamicFriction;

        /* Option 2: Average surface properties */
        // e = (A->restitution + B->restitution) / 2.0f;
        // staticFriction = (A->staticFriction + B->staticFriction) / 2.0f;
        // dynamicFriction = (A->dynamicFriction + B->dynamicFriction) / 2.0f;

        /* (3.5) Penetration depth -- Note: sign was flipped! 
         * 
         * Note: d is often already calculated when finding collisions, so
         *       it's already given as an argument. This is often (EPS) equal
         *       to the equation from (3.5).
         */
        // d = - dot((p1 - p2), n);

    }

    inline void update() {
        p1 = A->pose.p + A->pose.q * r1;
        p2 = B->pose.p + B->pose.q * r2;

        /* Recalculate N -- Not really required */
        // if (distance(p2, p1) > 0.001f) {
        //     n = normalize(p2 - p1);
        // }

        /* (3.5) Penetration depth */
        d = - dot((p1 - p2), n);
    }
};
