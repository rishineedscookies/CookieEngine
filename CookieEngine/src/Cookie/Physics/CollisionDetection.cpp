#include "ckpch.h"
#include "CollisionDetection.h"
#include "PhysicsShape.h"
#include "Cookie/Math/Geometry.h"
#include <mathfu/vector.h>
#include <mathfu/glsl_mappings.h>

namespace Cookie
{
	namespace Physics
	{
		bool TestAABBAABB(AABB* a, AABB* b)
		{
			if (std::abs(a->Center.y - b->Center.y) > (a->HalfExtents.y + b->HalfExtents.y)) return false;
			if (std::abs(a->Center.x - b->Center.x) > (a->HalfExtents.x + b->HalfExtents.x)) return false;
			if (std::abs(a->Center.z - b->Center.z) > (a->HalfExtents.z + b->HalfExtents.z)) return false;

			return true;
		}

		bool TestSphereSphere(Sphere* a, Sphere* b)
		{
			return mathfu::vec3::DotProduct(a->Center - b->Center, a->Center - b->Center) <= Math::Square(a->Radius + b->Radius);
		}

		bool TestSphereCapsule(Sphere* a, Capsule* b)
		{
			float dist = Math::SqDistPointSegment(b->A, b->B, a->Center);
			return dist <= Math::Square(a->Radius + b->Radius);
		}

		// TODO
		bool TestCapsuleCapsule(Capsule* a, Capsule* b)
		{
			return false;
		}

	}
}