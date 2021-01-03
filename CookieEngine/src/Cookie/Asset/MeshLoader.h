#pragma once

#include <string>

struct aiNode;
struct aiScene;
struct aiMesh;

namespace Cookie
{

	struct Mesh;
	struct Model;

	class MeshLoader
	{
	public:

		static Mesh* LoadMesh(const std::string& path);
		
		static Model* LoadModel(const std::string& path, bool bInstanced = false);

		static void LoadModel(const std::string& path, Model* model, bool bInstanced = false);

		static void ProcessNode(aiNode* node, const aiScene* scene, Model* model, bool bInstanced = false);
			
		static void ProcessMesh(aiMesh* loadedMesh, const aiScene* scene, Mesh* mesh, bool bInstanced = false);

		static void SetupMeshObjects(Mesh* mesh);

	};

}
