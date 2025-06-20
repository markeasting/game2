
#include "util/VectorMath.h"

vec2 rotateVec2(vec2 v, mat3 rotationMatrix) {
    vec3 v3 = vec3(v, 0.0);
    vec3 rotatedV3 = rotationMatrix * v3;
    return vec2(rotatedV3.x, rotatedV3.y);
}

quat QuatFromTwoVectors(vec3 vFrom, vec3 vTo) {
    return glm::rotation(glm::normalize(vFrom), glm::normalize(vTo));
}
