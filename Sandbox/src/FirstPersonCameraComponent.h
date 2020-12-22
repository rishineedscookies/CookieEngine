#pragma once

#include "Cookie/Renderer/OrthographicCamera.h"

#define FIRST_PERSON_CAMERA_ID 6

struct FirstPersonCameraComponent
{
	Cookie::OrthographicCamera Camera;
	mathfu::vec3 LocalPosition;
	mathfu::quat LocalRotation;
	mathfu::mat4 LocalTransform;
	bool bMainCamera;
	
	FirstPersonCameraComponent()
		: Camera(1.6f, -1.6f, -0.9, 0.9), bMainCamera(false) {}

	FirstPersonCameraComponent(float left, float right, float bottom, float top)
		: Camera(left, right, bottom, top),
		bMainCamera(true),
		LocalPosition(0.0f),
		LocalRotation(mathfu::quat::identity),
		LocalTransform(mathfu::mat4::Identity()) {}

};
