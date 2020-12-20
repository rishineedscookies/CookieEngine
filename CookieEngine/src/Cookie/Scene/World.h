#pragma once

#include "Cookie/Common/Vector.h"
#include "Cookie/Scene/Entity.h"
#include "Cookie/Scene/ComponentManager.h"

#include "Cookie/Core/Log.h"

#define GET_POOL(World, ComponentType, ID) World->GetComponentManager<ComponentType>(ID)
#define GET_COMPONENT(ComponentManager, idx) ComponentManager->GetComponent(idx) 

namespace Cookie {

	struct World
	{
		size_t EntityIndex = 1;
		Vector<size_t> SparseEntities = Vector<size_t>(5);
		size_t DenseIndex = 0;
		Vector<Entity> DenseEntities = Vector<Entity>(5);
		Vector<void*> ComponentManagers = Vector<void*>(5);
		uint32_t ComponentIDIndex = 1;
		
		Entity AddEntity();

		template<typename Component>
		Component* AddComponent(Entity e, uint32_t ComponentID);

		template<typename Component>
		void RegisterComponent(uint32_t ComponentID);

		template<typename Component>
		ComponentManager<Component>* GetComponentManager(size_t ComponentID);

		template<typename Component>
		bool EntityHasComponent(Entity e, ComponentManager<Component>* ComponentManager);
	};

	template<typename Component>
	Component* World::AddComponent(Entity e, uint32_t ComponentID)
	{
		ComponentManager<Component>* Manager = GetComponentManager<Component>(ComponentID);
		CK_CORE_ASSERT((e < SparseEntities.GetCapacity() || e == INVALID_ENTITY), "Tried to access entity out of bounds!");
		uint32_t Index = SparseEntities[e];
		Manager->ComponentPool->Add(Manager->Size);
		Manager->DenseEntities->Insert(e, Manager->Size);
		Manager->Entities->Insert(Manager->Size, e);
		return &(*Manager->ComponentPool)[Manager->Size++];
	}

	template<typename Component>
	void World::RegisterComponent(uint32_t ComponentID)
	{
		ComponentManager<Component>* Manager = new ComponentManager<Component>();
		ComponentManagers.Insert(Manager, ComponentID);
	}

	template<typename Component>
	ComponentManager<Component>* World::GetComponentManager(size_t ComponentID)
	{
		ComponentManager<Component>* Manager = reinterpret_cast<ComponentManager<Component>*>(ComponentManagers[ComponentID]);
		return Manager;
	}

	template<typename Component>
	bool World::EntityHasComponent(Entity e, ComponentManager<Component>* ComponentManager)
	{
		if (ComponentManager->Entities->GetCapacity() < e || e < 0 || e == INVALID_ENTITY)
			return false;

		size_t i = *ComponentManager->Entities->At(e);
		Entity t = *ComponentManager->DenseEntities->At(i);
		return t == e;
	}

}

