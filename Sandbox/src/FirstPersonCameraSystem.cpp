#include "FirstPersonCameraSystem.h"
#include "Cookie.h"

#include "InputSystem.h"
#include "TransformComponent.h"
#include "FirstPersonCameraComponent.h"

#include <mathfu/quaternion.h>

void FirstPersonCameraSystem::OnUpdate(Cookie::World* World, Cookie::Time* Time)
{
	PROFILE_SCOPE("Camera Update");
	auto Transforms = GET_POOL(World, TransformComponent, TRANSFORM_ID);
	auto Cameras = GET_POOL(World, FirstPersonCameraComponent, FIRST_PERSON_CAMERA_ID);
	for (int i = 0; i < Cameras->Size; i++)
	{
		Cookie::Entity idx = *Cameras->DenseEntities->At(i);
		if (!World->EntityHasComponent<TransformComponent>(idx, Transforms))
			continue;

		FirstPersonCameraComponent* Camera = GET_COMPONENT(Cameras, idx);
		TransformComponent* Transform = GET_COMPONENT(Transforms, idx);

		auto xRot = mathfu::quat::FromAngleAxis(-InputSystem::DeltaMouseX * 0.001f, mathfu::vec3(0.0f, 1.0f, 0.0f));
		Transform->Transform *= xRot.ToMatrix4();
		mathfu::vec3 right = mathfu::mat4::ToRotationMatrix(Transform->Transform) * mathfu::vec3(1.0f, 0, 0);
		auto yRot = mathfu::quat::FromAngleAxis(-InputSystem::DeltaMouseY  * 0.001f, mathfu::vec3(1.0f, 0.0f, 0.0f));
		Camera->LocalTransform *= yRot.ToMatrix4();
		mathfu::vec3 newPos = Transform->Transform.TranslationVector3D() + Camera->LocalPosition;
		Camera->Camera.SetPosition(newPos);
		Camera->Camera.SetRotation(mathfu::quat::FromMatrix(Transform->Transform * Camera->LocalTransform));

	}
}
