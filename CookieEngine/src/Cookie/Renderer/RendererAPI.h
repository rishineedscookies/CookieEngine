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

		virtual void Clear(const mathfu::vec4& color) const = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return s_RendererAPI; }
	private:
		static API s_RendererAPI;
	};

}