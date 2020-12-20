#pragma once

#include <mathfu/glsl_mappings.h>
#include <mathfu/matrix.h>

namespace Cookie
{

	struct PointLight
	{
		mathfu::vec3 Position;
		float Intensity;

		PointLight(mathfu::vec3 Position, float Intensity) :
			Position(Position), Intensity(Intensity) {}
	};

}
