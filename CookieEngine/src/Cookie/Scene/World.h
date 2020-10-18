#pragma once
#include "Cookie/Scene/ComponentManager.h"

namespace Cookie {

	typedef uint32_t Entity;

	struct World
	{
		uint32_t EntityIndex = 1;
		std::vector<size_t> SparseEntities = std::vector<size_t>(5);
		uint32_t DenseIndex = 0;
		std::vector<Entity> DenseEntities = std::vector<Entity>(5);
		std::vector<void*> ComponentManagers = std::vector<void*>(5);
		uint32_t ComponentIDIndex = 1;
		
		uint32_t AddEntity();

		template<typename Component>
		Component* AddComponent(uint32_t e, uint32_t ComponentID);

		template<typename Component>
		void RegisterComponent(uint32_t ComponentID);

		template<typename Component>
		ComponentManager<Component>* GetComponentManager(uint32_t ComponentID);
	};

	uint32_t World::AddEntity()
	{
		if (SparseEntities.capacity() <= EntityIndex)
		{
			SparseEntities.resize(EntityIndex * 2);
		}
		SparseEntities[EntityIndex] = DenseIndex;
		if (DenseEntities.capacity() <= DenseIndex)
		{
			DenseEntities.resize(DenseIndex * 2);
		}
		DenseEntities[DenseIndex] = EntityIndex;
		DenseIndex++;
		return EntityIndex++;
	}

	template<typename Component>
	Component* World::AddComponent(Entity e, uint32_t ComponentID)
	{
		ComponentManager<Component>* Manager = GetComponentManager<Component>(ComponentID);
		//CK_ASSERT(e < SparseEntities.size(), "Tried to access entity out of bounds!");
		uint32_t Index = SparseEntities[e];
		if (Manager->ComponentPool->capacity() <= Index)
		{
			Manager->ComponentPool->resize(Index * 2 + 1);
		}
		(*Manager->ComponentPool)[Index] = *(new Component());
		return &(*Manager->ComponentPool)[Index];
	}

	template<typename Component>
	void World::RegisterComponent(uint32_t ComponentID)
	{
		void* Manager = new ComponentManager<Component>();
		if (ComponentManagers.capacity() <= ComponentID)
		{
			ComponentManagers.reserve(ComponentID * 2);
		}
		ComponentManagers.insert(ComponentManagers.begin() + ComponentID, Manager);
	}

	template<typename Component>
	ComponentManager<Component>* World::GetComponentManager(uint32_t ComponentID)
	{
		ComponentManager<Component>* Manager = static_cast<ComponentManager<Component>*>(ComponentManagers.at(ComponentID));
		return Manager;
	}

}

