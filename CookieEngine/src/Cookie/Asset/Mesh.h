#pragma once

#include <mathfu/vector.h>
#include <mathfu/glsl_mappings.h>
#include <Cookie/Common/Vector.h>

namespace Cookie
{

	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;

	struct Vertex
	{
		mathfu::vec3 Position = mathfu::vec3(0.0f);
		mathfu::vec3 Normal = mathfu::vec3(0.0f);
		mathfu::vec2 TexCoords = mathfu::vec2(0.0f);
	};

	struct Mesh
	{
		Mesh() :
			Vertices(new Vector<Vertex>(1)),
			NumVertices(0),
			Indices(new Vector<uint32_t>(1)),
			NumIndices(0),
			VAO(NULL),
			VBO(NULL),
			EBO(NULL),
			InstancedModel(NULL),
			InstancedNormal(NULL)
		{}

		Vector<Vertex>* Vertices = new Vector<Vertex>(1);
		uint32_t NumVertices = 0;
		Vector<uint32_t>* Indices = new Vector<uint32_t>(1);
		uint32_t NumIndices = 0;
		VertexArray* VAO = NULL;
		VertexBuffer* VBO = NULL;
		IndexBuffer* EBO = NULL;
		bool bInstanced = false;
		VertexBuffer* InstancedModel = NULL;
		VertexBuffer* InstancedNormal = NULL;
	};

	struct Model
	{
		Vector<Mesh>* Meshes = new Vector<Mesh>(1);
		uint32_t NumMeshes = 0;
		bool bInstanced = false;
	};

	struct ModelManager
	{
		Vector<Model*> Models = Vector<Model*>(1);
		uint32_t NumModels = 0;

		~ModelManager();

		Model* LoadModel(const std::string& path);
		Model* LoadInstancedModel(const std::string& path);
	};


}
