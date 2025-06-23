#pragma once

#include "phys/RigidBody.h"

struct CollisionPair {
    RigidBody* A = nullptr;
    RigidBody* B = nullptr;

    CollisionPair(
        RigidBody* A, 
        RigidBody* B
    ) : A(A), B(B) {

        assert(A != nullptr);
        assert(B != nullptr);

        const float vrel = glm::length2(A->vel - B->vel);
        
        /* Wake sleeping bodies if a collision could occur */
        if (vrel > 0.01f) {
            A->wake();
            B->wake();
        }
    }
};
