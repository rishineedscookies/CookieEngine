#pragma once

#include "Cookie/Common/Vector.h"
#include "Cookie/Scene/Entity.h"

namespace Cookie {

	template<typename Component>
	struct ComponentManager
	{
		Vector<Component>* ComponentPool = new Vector<Component>(1);
		Vector<Entity>* DenseEntities = new Vector<Entity>(1);
		Vector<size_t>* Entities = new Vector<size_t>(1);
		uint32_t Size = 0;

		inline bool ContainsEntity(Entity e);
	};

	template<typename Component>
	bool ComponentManager<Component>::ContainsEntity(Entity e)
	{
		return false;
		//return DenseEntities->At(Entities->At((size_t) e) = e;
	}

}
