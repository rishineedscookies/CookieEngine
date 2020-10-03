#pragma once

#include "ckpch.h"

namespace Cookie {

	typedef uint32_t Entity;

	template<typename ComponentType>
	struct ComponentManager
	{

		struct ComponentData
		{
			unsigned int Size = 1;
			ComponentType Data;
		};

		ComponentData DataList;
		std::array<uint32_t, 1024> EntityMap = std::array(-1);
		
		uint32_t AddComponent(Entity e, ComponentType& comp);

		ComponentType* FindComponent(Entity e);

		void DestroyComponent(Entity e);
	};

	template<typename ComponentType>
	uint32_t Cookie::ComponentManager<ComponentType>::AddComponent(Entity e, ComponentType& comp)
	{
		CK_ASSERT(EntityMap[e] == -1, "Entity {0} already has this component!");
		
		
	}

}
