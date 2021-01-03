#include "ckpch.h"
#include "MeshLoader.h"
#include "Mesh.h"
#include "Cookie/Renderer/Buffer.h"
#include "Cookie/Renderer/VertexArray.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Cookie
{

	Mesh* MeshLoader::LoadMesh(const std::string& path)
	{
		return NULL;
	}

	Model* MeshLoader::LoadModel(const std::string& path, bool bInstanced)
	{
		Model* model = new Model();
		LoadModel(path, model, bInstanced);
		return model;
	}

	void MeshLoader::LoadModel(const std::string& path, Model* model, bool bInstanced)
	{
		model->bInstanced = bInstanced;
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			CK_CORE_ERROR("ERROR LOADING MODEL: {0}", importer.GetErrorString());
			return;
		}
		ProcessNode(scene->mRootNode, scene, model, bInstanced);
	}

	void MeshLoader::ProcessNode(struct aiNode* node, const struct aiScene* scene, Model* model, bool bInstanced)
	{
		for (uint32_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* loadedMesh = scene->mMeshes[node->mMeshes[i]];
			Mesh* mesh = (*model->Meshes).At(i);
			ProcessMesh(loadedMesh, scene, mesh, bInstanced);
			model->NumMeshes++;
		}

		for (uint32_t i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene, model, bInstanced);
		}
	}

	void MeshLoader::ProcessMesh(struct aiMesh* loadedMesh, const struct aiScene* scene, Mesh* mesh, bool bInstanced)
	{
		// Process vertices
		for (uint32_t i = 0; i < loadedMesh->mNumVertices; i++)
		{
			Vertex vertex;
			vertex.Position.x = loadedMesh->mVertices[i].x;
			vertex.Position.y = loadedMesh->mVertices[i].y;
			vertex.Position.z = loadedMesh->mVertices[i].z;
			vertex.Normal.x = loadedMesh->mNormals[i].x;
			vertex.Normal.y = loadedMesh->mNormals[i].y;
			vertex.Normal.z = loadedMesh->mNormals[i].z;
			if(loadedMesh->mTextureCoords[0])
			{
				vertex.TexCoords.x = loadedMesh->mTextureCoords[0][i].x;
			}
			else
			{
				vertex.TexCoords = mathfu::vec2(0.0f);
			}
			mesh->Vertices->Insert(vertex, i);
			mesh->NumVertices++;
		}
		
		// Process faces

		uint32_t faceIndex = 0;
		for (uint32_t i = 0; i < loadedMesh->mNumFaces; i++)
		{
			aiFace face = loadedMesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; j++)
			{
				mesh->Indices->Insert(face.mIndices[j], faceIndex++);
				mesh->NumIndices++;
			}
		}
		mesh->bInstanced = bInstanced;
		SetupMeshObjects(mesh);
	}

	void MeshLoader::SetupMeshObjects(Mesh* mesh)
	{
		mesh->VBO = VertexBuffer::Create(mesh->NumVertices * sizeof(Vertex));
		mesh->VBO->SetData(mesh->Vertices->GetData(), mesh->NumVertices * sizeof(Vertex));
		mesh->VBO->SetLayout({
				{ ShaderDataType::Float3, "a_Position"},
				{ ShaderDataType::Float3, "a_Normal"},
				{ ShaderDataType::Float2, "a_UV"},
			});
		mesh->VBO->SetData((void*) &(*mesh->Vertices)[0], mesh->NumIndices);
		mesh->VAO = VertexArray::Create();
		mesh->VAO->AddVertexBuffer(mesh->VBO);
		mesh->EBO = IndexBuffer::Create(&(*mesh->Indices)[0], mesh->NumIndices);
		mesh->VAO->SetIndexBuffer(mesh->EBO);

		/*if (mesh->bInstanced)
		{
			mesh->InstancedModel = VertexBuffer::Create(400 * 64);
			BufferLayout layout = BufferLayout({
				{ ShaderDataType::Float4, "a_MVP", 0, false, 1},
				{ ShaderDataType::Float4, "a_MVP", 16, false, 1},
				{ ShaderDataType::Float4, "a_MVP", 32, false, 1},
				{ ShaderDataType::Float4, "a_MVP", 48, false, 1}
			}, 3);
			mesh->InstancedModel->SetLayout(layout);
			mesh->VAO->AddVertexBuffer(mesh->InstancedModel);
		}*/
	}

}