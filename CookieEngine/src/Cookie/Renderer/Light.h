#pragma once

#include <mathfu/glsl_mappings.h>
#include <mathfu/matrix.h>

namespace Cookie
{

	struct DirectionalLight
	{
		mathfu::vec3 Direction;
		mathfu::vec3 Diffuse;

		DirectionalLight(mathfu::vec3 Direction, mathfu::vec3 Diffuse) :
			Direction(Direction), Diffuse(Diffuse) {}

	};

	struct PointLight
	{
		mathfu::vec3 Position;
		mathfu::vec3 Diffuse;

		PointLight(mathfu::vec3 Position, mathfu::vec3 Diffuse) :
			Position(Position), Diffuse(Diffuse) {}
	};

}
