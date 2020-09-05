#pragma once
#include "Cookie/Renderer/RendererAPI.h"

namespace Cookie {

	class OpenGLRendererAPI : public RendererAPI
	{
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, int32_t y, uint32_t width, uint32_t height) override;
		virtual void Clear(const mathfu::vec4& color) const override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;

	};

}