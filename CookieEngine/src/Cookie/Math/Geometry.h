#pragma once

#include <mathfu/vector.h>
#include <mathfu/glsl_mappings.h>

namespace Cookie
{
	namespace Math
	{

		inline float Square(float n)
		{
			return n * n;
		}

		inline float SqDistPointSegment(mathfu::vec3 a, mathfu::vec3 b, mathfu::vec3 p)
		{
			mathfu::vec3 line = (b - a).Normalized();
			mathfu::vec3 diff = p - a;
			float t = mathfu::dot(line, diff);
			if (t <= 0.0f) return diff.LengthSquared();
			if (t >= (b - a).LengthSquared()) return (p - b).LengthSquared();
			return (diff - line * t).LengthSquared();
		}
	}

}
