#pragma once

#include <mathfu/vector.h>
#include <vectorial/vec4f.h>
#include <mathfu/glsl_mappings.h>

#define MOVEMENT_ID 3

struct MovementComponent
{
	mathfu::vec3 Acceleration = mathfu::vec3(0.0f);
	mathfu::vec3 Velocity = mathfu::vec3(0.0f);
};
