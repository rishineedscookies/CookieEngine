#pragma once

namespace Cookie
{
	namespace Physics
	{
		struct AABB;
		struct Sphere;
		struct Capsule;
		struct OOBB;

		bool TestAABBAABB(AABB* a, AABB* b);

		bool TestSphereSphere(Sphere* a, Sphere* b);

		bool TestSphereCapsule(Sphere* a, Capsule* b);

		bool TestCapsuleCapsule(Capsule* a, Capsule* b);
	}
}
