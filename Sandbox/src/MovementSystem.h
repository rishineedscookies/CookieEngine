#pragma once

namespace Cookie
{
	struct World;
	struct Time;
}

struct MovementSystem
{
	void OnUpdate(Cookie::World* World, Cookie::Time* Time);
};

