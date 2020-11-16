#pragma once

#include <mathfu/vector.h>
#include <vectorial/vec4f.h>
#include <mathfu/glsl_mappings.h>

#define AABB_COLLIDER_ID 2

struct AABBColliderComponent
{
	mathfu::vec3 Offset = mathfu::vec3(0.0f);
	mathfu::vec3 HalfExtents = mathfu::vec3(0.0f);
};
