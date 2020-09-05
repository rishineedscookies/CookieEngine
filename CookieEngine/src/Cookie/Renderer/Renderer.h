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

		static void BeginScene(OrthographicCamera& camera); // TODO: Take in scene params
		static void EndScene();

		static void Submit(const Ref<Shader> shader, const Ref<VertexArray>& vertexArray, const mathfu::mat4& transform = mathfu::mat4::Identity());


		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData {
			mathfu::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};

}