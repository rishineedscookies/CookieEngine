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

		Movement->Velocity.x = 0.0f;
		Movement->Velocity.z = 0.0f;
		mathfu::vec2 input(0.0f);
		if (Cookie::Input::GetKeyDown(CK_KEY_LEFT))
		{
			input.x = -1.0f;
		}
		if (Cookie::Input::GetKeyDown(CK_KEY_UP))
		{
			input.y = 1.0f;
		}
		if (Cookie::Input::GetKeyDown(CK_KEY_RIGHT))
		{
			input.x = 1.0f;
		}
		if (Cookie::Input::GetKeyDown(CK_KEY_DOWN))
		{
			input.y = -1.0f;
		}
		if (Cookie::Input::GetKeyDown(CK_KEY_SPACE))
		{
			Movement->Velocity.y = 2.5f;
		}
		mathfu::vec3 forward = mathfu::mat4::ToRotationMatrix(Transform->Transform) * mathfu::vec3(0.0f, 0.0f, 1.0f);
		mathfu::vec3 right = mathfu::mat4::ToRotationMatrix(Transform->Transform) * mathfu::vec3(1.0f, 0.0f, 0.0f);
		if (input.LengthSquared() != 0.0f) {
		}
		mathfu::vec3 Acceleration = forward * -input.y +
			right * input.x;
		CK_TRACE("Velocity: {0}, {1}, {2}", Movement->Velocity.x, Movement->Velocity.y, Movement->Velocity.z);
		Movement->Velocity.x = Acceleration.x; //* Time->DeltaTime;
		Movement->Velocity.y -= 2.0f * Time->DeltaTime;
		Movement->Velocity.z = Acceleration.z; //* Time->DeltaTime;

		constexpr uint32_t iterations = 6;
		for (uint32_t itr = 0; itr < 6; itr++)
		{
			float ItrTime = Time->DeltaTime / iterations;
			mathfu::mat4 OldTransform = Transform->Transform;
			Transform->Transform = mathfu::mat4::FromTranslationVector(Movement->Velocity * Time->DeltaTime) * Transform->Transform;
			HitResult Hit;
			if (PhysicsSystem::CheckIntersection(World, idx, Hit))
			{
				mathfu::vec3 VelInNorm = -mathfu::vec3::DotProduct(Movement->Velocity, Hit.Normal) * Hit.Normal;
				Movement->Velocity += VelInNorm;
				Movement->Velocity += Hit.Penetration * Hit.Normal;
				
				Transform->Transform = mathfu::mat4::FromTranslationVector(Movement->Velocity * Time->DeltaTime) * OldTransform;
			}
		}
	}
}
