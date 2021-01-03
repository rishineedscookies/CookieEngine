#pragma once

#include "RendererAPI.h"

namespace Cookie {

	class RenderCommand {
	public:

		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void SetViewport(uint32_t x, int32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void Clear(const mathfu::vec4& color = mathfu::vec4(1.0f, 1.0f, 1.0f, 1.0f))
		{
			s_RendererAPI->Clear(color);
		}

		inline static void DrawIndexed(const VertexArray* vertexArray, uint32_t count = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, count);
		}

		inline static void DrawIndexedInstanced(const VertexArray* vertexArray, uint32_t count = 0, uint32_t instances = 0)
		{
			s_RendererAPI->DrawIndexedInstanced(vertexArray, count, instances);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};

}