#include "PhysicsSystem.h"
#include <Cookie.h>
#include "AABBColliderComponent.h"
#include "TransformComponent.h"


void PhysicsSystem::OnUpdate(Cookie::World* World, Cookie::Time* Time)
{
	PROFILE_SCOPE("Physics Update");
	Cookie::ComponentManager<AABBColliderComponent>* AABBColliders = GET_POOL(World, AABBColliderComponent, AABB_COLLIDER_ID);
	Cookie::ComponentManager<TransformComponent>* Transforms = GET_POOL(World, TransformComponent, TRANSFORM_ID);
	for (int i = 0; i < AABBColliders->Size; i++)
	{
		for (int j = 0; j < i; j++)
		{
			Cookie::Entity A = (*AABBColliders->DenseEntities)[i];
			Cookie::Entity B = (*AABBColliders->DenseEntities)[j];

			if(!(A && B))
				continue;

			if(A == B)
				continue;
			
			AABBColliderComponent* AColl = GET_COMPONENT(AABBColliders, A);
			TransformComponent* ATrans = GET_COMPONENT(Transforms, A);	
			AABBColliderComponent* BColl = GET_COMPONENT(AABBColliders, B);
			TransformComponent* BTrans = GET_COMPONENT(Transforms, B);
			if ( AColl == NULL || ATrans == NULL || BColl == NULL || BTrans == NULL)
				continue;
			
			//if (CheckCollision(AColl, ATrans, BColl, BTrans))
				//CK_CORE_INFO("Collision Detected! {0} and {1}", A, B);
		}
	}
}

bool PhysicsSystem::CheckCollision(struct AABBColliderComponent* A, struct TransformComponent* ATrans, struct AABBColliderComponent* B, struct TransformComponent* BTrans)
{
	mathfu::vec3 APos = ATrans->Transform.TranslationVector3D();
	mathfu::vec3 BPos = BTrans->Transform.TranslationVector3D();
	return (A->Offset.x + A->HalfExtents.x + APos.x > B->Offset.x - B->HalfExtents.x + BPos.x)
		&& (A->Offset.y + A->HalfExtents.y + APos.y > B->Offset.y - B->HalfExtents.y + BPos.y)
		&& (A->Offset.x + A->HalfExtents.x + APos.z > B->Offset.z - B->HalfExtents.z + BPos.z)
		&& (A->Offset.x - A->HalfExtents.x + APos.x < B->Offset.x + B->HalfExtents.x + BPos.x)
		&& (A->Offset.y - A->HalfExtents.y + APos.y < B->Offset.y + B->HalfExtents.y + BPos.y)
		&& (A->Offset.z - A->HalfExtents.z + APos.z < B->Offset.x + B->HalfExtents.x + BPos.z);
}

bool PhysicsSystem::CheckIntersection(Cookie::World* World, Cookie::Entity A, Cookie::Entity B, HitResult& OutHitResult)
{
	const Cookie::ComponentManager<AABBColliderComponent>* AABBColliders = GET_POOL(World, AABBColliderComponent, AABB_COLLIDER_ID);
	const Cookie::ComponentManager<TransformComponent>* Transforms = GET_POOL(World, TransformComponent, TRANSFORM_ID);
	const TransformComponent* ATrans = Transforms->GetComponent(A);//GET_COMPONENT(Transforms, A);
	const TransformComponent* BTrans = Transforms->GetComponent(B);
	const AABBColliderComponent* ACollider = AABBColliders->GetComponent(A);
	const AABBColliderComponent* BCollider = AABBColliders->GetComponent(B);
	return CheckIntersection(ACollider, ATrans, BCollider, BTrans, OutHitResult);
}

bool PhysicsSystem::CheckIntersection(const struct AABBColliderComponent* ACollider, const struct TransformComponent* ATrans, const struct AABBColliderComponent* BCollider, const struct TransformComponent* BTrans, HitResult& OutHitResult)
{
	const mathfu::vec3 APos = ATrans->Transform.TranslationVector3D();
	const mathfu::vec3 BPos = BTrans->Transform.TranslationVector3D();

	CK_CORE_ASSERT(&APos, "Position for entity A not found!");
	CK_CORE_ASSERT(&BPos, "Position for entity B not found!");
	CK_CORE_ASSERT(ACollider , "Collider for entity A not found!");
	CK_CORE_ASSERT(BCollider, "Collider for entity B not found!");

	const mathfu::vec3 Offset = BPos - APos;
	const float XOverlap = ACollider->HalfExtents.x + BCollider->HalfExtents.x - abs(Offset.x);
	
	if (XOverlap > 0)
	{
		const float YOverlap = ACollider->HalfExtents.y + BCollider->HalfExtents.y - abs(Offset.y);
		if (YOverlap > 0)
		{
			const float ZOverlap = ACollider->HalfExtents.z + BCollider->HalfExtents.z - abs(Offset.z);
			if (ZOverlap > 0)
			{
				if (XOverlap < YOverlap && XOverlap < ZOverlap)
				{
					OutHitResult.Normal = mathfu::vec3(Offset.x < 0 ? 1.0f : -1.0f, 0.0f, 0.0f);
					OutHitResult.Penetration = XOverlap;
				}
				else if (YOverlap < XOverlap && YOverlap < ZOverlap)
				{
					OutHitResult.Normal = mathfu::vec3(0.0f, Offset.y < 0 ? 1.0f : -1.0f, 0.0f);
					OutHitResult.Penetration = YOverlap;
				}
				else if (ZOverlap < YOverlap && ZOverlap < XOverlap)
				{
					OutHitResult.Normal = mathfu::vec3(0.0f, 0.0f, Offset.z < 0 ? 1.0f : -1.0f);
					OutHitResult.Penetration = ZOverlap;
				}
				return true;
			}
		}
	}
	return false;

}

bool PhysicsSystem::CheckIntersection(Cookie::World* World, Cookie::Entity A, HitResult& OutHitResult)
{
	Cookie::ComponentManager<TransformComponent>* Transforms = GET_POOL(World, TransformComponent, TRANSFORM_ID);
	Cookie::ComponentManager<AABBColliderComponent>* Colliders = GET_POOL(World, AABBColliderComponent, AABB_COLLIDER_ID);
	for (int i = 0; i < Colliders->Size; i++)
	{
		Cookie::Entity B = (*Colliders->DenseEntities)[i];
		if (A == B)
			continue;

		if (CheckIntersection(World, A, B, OutHitResult))
		{
			return true;
		}
	}
	return false;
}
