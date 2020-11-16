#pragma once

#include <mathfu/glsl_mappings.h>

namespace Cookie
{
	struct World;
	struct Time;
	typedef unsigned int uint32_t;
	typedef uint32_t Entity;
}

struct HitResult
{
	mathfu::Vector<float, 3> Normal;
	float Penetration;
};

struct PhysicsSystem
{
	void OnUpdate(Cookie::World* World, Cookie::Time* Time);

	bool CheckCollision(struct AABBColliderComponent* A, struct TransformComponent* ATrans, struct AABBColliderComponent* B, struct TransformComponent* BTrans);

	static bool CheckIntersection(Cookie::World* World, Cookie::Entity A, HitResult& OutHitResult);
	static bool CheckIntersection(Cookie::World* World, Cookie::Entity A, Cookie::Entity B, HitResult& OutHitResult);
	static bool CheckIntersection(const struct AABBColliderComponent* ACollider, const struct TransformComponent* ATrans, const struct AABBColliderComponent* BCollider, const struct TransformComponent* BTrans, HitResult& OutHitResult);
}; 

