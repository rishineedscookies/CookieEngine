#pragma once
#include "Cookie/Renderer/RendererAPI.h"

namespace Cookie {

	class OpenGLRendererAPI : public RendererAPI
	{

		virtual void Clear(const mathfu::vec4& color) const override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;

	};

}