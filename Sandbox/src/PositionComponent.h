#pragma once

#include <mathfu/vector.h>
#include <vectorial/vec4f.h>
#include <mathfu/glsl_mappings.h>

#define POSITION_ID 1

struct PositionComponent
{
	mathfu::vec3 Position = mathfu::vec3(0.0f);
};
