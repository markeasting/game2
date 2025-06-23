#pragma once

/** @todo maybe change this to compiler flags using -DGLM_FORCE_INTRINSICS etc. */
#define GLM_FORCE_CXX20
#define GLM_FORCE_INTRINSICS // @TODO test performance
// #define GLM_FORCE_INLINE // @TODO test performance
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>

// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtx/vector_angle.hpp>

using namespace glm;

/* @todo add double precision to XPBD solver */
// typedef glm::dvec2		vec2;
// typedef glm::dvec3		vec3;
// typedef glm::dvec4		vec4;
// typedef glm::dmat3      mat3;
// typedef glm::dmat4      mat4;
// typedef glm::dquat      quat;

// typedef double          scalar;
