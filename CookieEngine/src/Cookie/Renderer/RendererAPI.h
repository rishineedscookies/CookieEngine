#pragma once

#include <mathfu/vector.h>
#include <mathfu/glsl_mappings.h>

#include "VertexArray.h"

namespace Cookie {

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL
		};
	public:
		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, int32_t y, uint32_t width, uint32_t height) = 0;
		virtual void Clear(const mathfu::vec4& color) const = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0) = 0;

		inline static API GetAPI() { return s_RendererAPI; }
	private:
		static API s_RendererAPI;
	};

}