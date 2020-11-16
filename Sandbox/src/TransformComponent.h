#pragma once

#include <mathfu/matrix.h>
#include <mathfu/glsl_mappings.h>

#define TRANSFORM_ID 4

struct TransformComponent
{
	mathfu::mat4 Transform = mathfu::mat4::Identity();
};
