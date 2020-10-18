#pragma once

#include <vector>

namespace Cookie {

	template<typename Component>
	struct ComponentManager
	{
		std::vector<Component>* ComponentPool = new std::vector<Component>;
	};

}
