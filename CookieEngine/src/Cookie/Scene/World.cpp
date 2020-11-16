#include "ckpch.h"

#include "World.h"

namespace Cookie {

	Entity World::AddEntity()
	{
		SparseEntities.Insert(DenseIndex, EntityIndex);
		DenseEntities.Insert(EntityIndex, DenseIndex);
		DenseIndex++;
		return EntityIndex++;
	}

}