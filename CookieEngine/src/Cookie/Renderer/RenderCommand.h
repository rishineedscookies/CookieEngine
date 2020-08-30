#pragma once

#include "RendererAPI.h"

namespace Cookie {

	class RenderCommand {
	public:

		inline static void Clear(const mathfu::vec4& color = mathfu::vec4(1.0f, 1.0f, 1.0f, 1.0f))
		{
			s_RendererAPI->Clear(color);
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray> vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};

}