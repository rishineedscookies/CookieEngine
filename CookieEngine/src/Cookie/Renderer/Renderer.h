#pragma once

#include "RenderCommand.h"

#include "OrthographicCamera.h"
#include "Cookie/Common/Vector.h"
#include "Shader.h"

namespace Cookie {

	class Renderer
	{
		struct MeshInstance
		{
			MeshInstance()
				: Mat(NULL), Instance(NULL), NumInstances(0), ModelMatrices(new Vector<mathfu::mat4>(1)), NormalMatrices(new Vector<mathfu::mat4>(1))
			{}

			MeshInstance(class Material* mat, struct Mesh* mesh)
				: Mat(mat), Instance(mesh), NumInstances(0), ModelMatrices(new Vector<mathfu::mat4>(1)), NormalMatrices(new Vector<mathfu::mat4>(1))
			{}

			~MeshInstance()
			{
				delete ModelMatrices;
				delete NormalMatrices;
			}
			class Material* Mat;
			struct Mesh* Instance;
			uint32_t NumInstances;
			Vector<mathfu::mat4>* ModelMatrices;
			Vector<mathfu::mat4>* NormalMatrices;
		};

	public:
		
		static void Init();
		static void OnFrameBufferResize(uint32_t width, uint32_t height);

		static void BeginScene(OrthographicCamera& camera, struct PointLight* pointLight, struct DirectionalLight* directionalLight); // TODO: Take in scene params
		static void EndScene();

		static void Submit(const Ref<Shader> shader, const VertexArray* vertexArray, const mathfu::mat4& transform = mathfu::mat4::Identity());

		static void DrawModel(struct Model* model, class Material* material, const mathfu::mat4* transform);
		static void DrawMesh(struct Mesh* mesh, class Material* material, const mathfu::mat4* transform);
		static void SubmitMeshInstance(struct Mesh* mesh, class Material* material, const mathfu::mat4* transform);


		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:

		static void UploadSceneUniforms(Shader* shader);

		static MeshInstance* GetMeshInstance(struct Mesh* mesh, class Material* material);

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
		static MeshInstance* m_MeshInstances;
		static uint32_t m_NumInstances;
	};

}