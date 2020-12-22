#pragma once

namespace Cookie
{
	struct World;
	struct Time;
}

struct InputSystem
{
	static void OnUpdate(Cookie::World* World, Cookie::Time* Time);

	static float InputSystem::CurrMouseX;
	static float InputSystem::CurrMouseY;
	static float InputSystem::DeltaMouseX;
	static float InputSystem::DeltaMouseY;
};

