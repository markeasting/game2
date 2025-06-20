#pragma once

#include "common/glm.h"

vec2 rotateVec2(vec2 v, mat3 rotationMatrix);

quat QuatFromTwoVectors(vec3 vFrom, vec3 vTo);
