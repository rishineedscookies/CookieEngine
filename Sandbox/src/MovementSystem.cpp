#include "MovementSystem.h"
#include <Cookie.h>

#include "TransformComponent.h"
#include "MovementComponent.h"
#include "AABBColliderComponent.h"
#include "PhysicsSystem.h"

void MovementSystem::OnUpdate(Cookie::World* World, Cookie::Time* Time)
{
	Cookie::ComponentManager<TransformComponent>* Transforms = GET_POOL(World, TransformComponent, TRANSFORM_ID);
	Cookie::ComponentManager<MovementComponent>* Movements = GET_POOL(World, MovementComponent, MOVEMENT_ID);
	Cookie::ComponentManager<AABBColliderComponent>* Colliders = GET_POOL(World, AABBColliderComponent, AABB_COLLIDER_ID);
	for (int i = 0; i < Movements->Size; i++)
	{
		Cookie::Entity idx = *Movements->DenseEntities->At(i);
		if (!World->EntityHasComponent<TransformComponent>(idx, Transforms) ||
			!World->EntityHasComponent<AABBColliderComponent>(idx, Colliders))
			continue;

		MovementComponent* Movement = GET_COMPONENT(Movements, idx);
		TransformComponent* Transform = GET_COMPONENT(Transforms, idx);
		AABBColliderComponent* Collider = GET_COMPONENT(Colliders, idx);

		Movement->Velocity = mathfu::vec3(0.0f);
		if (Cookie::Input::GetKeyDown(CK_KEY_LEFT))
		{
			Movement->Velocity.x = -1.0f;
		}
		if (Cookie::Input::GetKeyDown(CK_KEY_UP))
		{
			Movement->Velocity.y = 1.0f;
		}
		if (Cookie::Input::GetKeyDown(CK_KEY_RIGHT))
		{
			Movement->Velocity.x = 1.0f;
		}
		if (Cookie::Input::GetKeyDown(CK_KEY_DOWN))
		{
			Movement->Velocity.y = -1.0f;
		}
		
		mathfu::mat4 OldTransform = Transform->Transform;
		Transform->Transform *= mathfu::mat4::FromTranslationVector(Movement->Velocity * Time->DeltaTime);
		HitResult Hit;
		if (PhysicsSystem::CheckIntersection(World, idx, Hit))
		{
			mathfu::vec3 VelInNorm = -mathfu::vec3::DotProduct(Movement->Velocity, Hit.Normal) * Hit.Normal;
			CK_INFO("Penetration: {0}", Hit.Penetration);
			Movement->Velocity += VelInNorm;
			Movement->Velocity += Hit.Penetration * Hit.Normal;
			
			Transform->Transform = OldTransform * mathfu::mat4::FromTranslationVector(Movement->Velocity * Time->DeltaTime);
		}
	}
}
