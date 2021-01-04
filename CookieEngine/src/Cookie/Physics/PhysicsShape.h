#pragma once

#include <mathfu/vector.h>
#include <mathfu/glsl_mappings.h>

namespace Cookie
{
	namespace Physics
	{
		struct AABB
		{
			mathfu::vec3 Center;
			mathfu::vec3 HalfExtents;
		};

		struct Sphere
		{
			mathfu::vec3 Center;
			float Radius;
		};

		struct Capsule
		{
			mathfu::vec3 A;
			mathfu::vec3 B;
			float Radius;
		};

		struct OOBB
		{
			mathfu::vec3 Center;
			mathfu::mat3 Axes;
			mathfu::vec3 HalfExtents;
		};
	}
}