#pragma once
#include "Cookie/Renderer/RendererAPI.h"

namespace Cookie {

	class OpenGLRendererAPI : public RendererAPI
	{
		
		//void OpenGLDebugMessageCallback(unsigned source, unsigned type, unsigned id, unsigned severity, int length, const char* message, const void* userParam);

		virtual void Init() override;
		virtual void SetViewport(uint32_t x, int32_t y, uint32_t width, uint32_t height) override;
		virtual void Clear(const mathfu::vec4& color) const override;

		virtual void DrawIndexed(const VertexArray* vertexArray, uint32_t count = 0) override;
		virtual void DrawIndexedInstanced(const VertexArray* vertexArray, uint32_t count = 0, uint32_t instances = 0) override;

	};

}