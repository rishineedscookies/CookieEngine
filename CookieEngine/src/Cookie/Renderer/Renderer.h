#pragma once

#include "RenderCommand.h"

#include "OrthographicCamera.h"
#include "Shader.h"

namespace Cookie {

	class Renderer
	{
	public:
		
		static void Init();
		static void OnFrameBufferResize(uint32_t width, uint32_t height);

		static void BeginScene(OrthographicCamera& camera, struct PointLight* pointLight, struct DirectionalLight* directionalLight); // TODO: Take in scene params
		static void EndScene();

		static void Submit(const Ref<Shader> shader, const VertexArray* vertexArray, const mathfu::mat4& transform = mathfu::mat4::Identity());

		static void DrawModel(const struct Model* model, class Material* material, const mathfu::mat4* transform);
		static void DrawMesh(const struct Mesh* mesh, class Material* material, const mathfu::mat4* transform);


		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:

		static void UploadSceneUniforms(Shader* shader);

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
	};

}