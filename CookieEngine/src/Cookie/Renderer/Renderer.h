#pragma once

#include "RenderCommand.h"

#include "OrthographicCamera.h"
#include "Shader.h"

namespace Cookie {

	class Renderer
	{
		struct InstancedMesh
		{
			~InstancedMesh()
			{
				delete[] VertexBufferBase;
				delete[] IndexBufferBase;
			}
			class Material* Mat;
			struct Mesh* Instance;
			const uint32_t MaxTris = 20000;
			const uint32_t MaxIndices = MaxTris * 3;
			uint32_t VertexCount = 0;
			uint32_t IndexCount = 0;
			struct Vertex* VertexBufferBase;
			struct Vertex* VertexBufferPtr;
			uint32_t* IndexBufferBase;
			uint32_t* IndexBufferPtr;
		};

	public:
		
		static void Init();
		static void OnFrameBufferResize(uint32_t width, uint32_t height);

		static void BeginScene(OrthographicCamera& camera, struct PointLight* pointLight, struct DirectionalLight* directionalLight); // TODO: Take in scene params
		static void EndScene();

		static void Submit(const Ref<Shader> shader, const VertexArray* vertexArray, const mathfu::mat4& transform = mathfu::mat4::Identity());

		static void DrawModel(struct Model* model, class Material* material, const mathfu::mat4* transform);
		static void DrawMesh(struct Mesh* mesh, class Material* material, const mathfu::mat4* transform);
		static void SubmitInstanceMesh(struct Mesh* mesh, class Material* material, const mathfu::mat4* transform);


		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:

		static void UploadSceneUniforms(Shader* shader);

		static InstancedMesh* GetMeshInstance(struct Mesh* mesh, class Material* material);

	private:
		struct SceneData {
			mathfu::mat4 ViewProjectionMatrix;
			mathfu::vec3 ViewPosition;
			mathfu::vec3 PointLightPosition;
			mathfu::vec3 PointLightDiffuse;
			mathfu::vec3 DirectionalLightDirection;
			mathfu::vec3 DirectionalLightDiffuse;
		};

		

		static SceneData* m_SceneData;
		static InstancedMesh* m_MeshInstances;
		static uint32_t m_NumMats;
	};

}