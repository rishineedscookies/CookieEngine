#pragma once

namespace Cookie
{
	struct World;
	struct Time;
}

struct FirstPersonCameraSystem
{
	void OnUpdate(Cookie::World* World, Cookie::Time* Time);
};

