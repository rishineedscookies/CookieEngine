#pragma once

#include "RenderCommand.h"

#include "OrthographicCamera.h"
#include "Shader.h"

namespace Cookie {

	class Renderer
	{
	public:

		static void BeginScene(OrthographicCamera& camera); // TODO: Take in scene params
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader> shader, const std::shared_ptr<VertexArray>& vertexArray);


		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData {
			mathfu::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};

}