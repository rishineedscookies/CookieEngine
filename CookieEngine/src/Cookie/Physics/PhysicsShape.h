#pragma once

#include <mathfu/vector.h>
#include <mathfu/glsl_mappings.h>

struct AABBCollider
{
	mathfu::vec3 Center;
	mathfu::vec3 HalfExtents;
};

struct SphereCollider
{
	mathfu::vec3 Center;
	float Radius;
};


