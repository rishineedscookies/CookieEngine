#include "ckpch.h"
#include "Mesh.h"
#include "MeshLoader.h"

namespace Cookie
{

	ModelManager::~ModelManager()
	{

	}

	Model* ModelManager::LoadModel(const std::string& path)
	{
		if (Models.GetCapacity() <= NumModels)
		{
			Models.Reserve(NumModels * 2 + 1);
		}
		Model* model = *Models.At(NumModels);
		model = new Model();
		MeshLoader::LoadModel(path, model);
		NumModels++;
		return model;
	}

}