#pragma once

#include "Cookie/Renderer/VertexArray.h"

namespace Cookie {

	class VertexBuffer;
	class IndexBuffer;
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void AddVertexBuffer(const VertexBuffer* vertexBuffer) override;
		virtual void SetIndexBuffer(const IndexBuffer* indexBuffer)  override;
		virtual const std::vector<VertexBuffer*>& GetVertexBuffers() const override { return m_VertexBuffers; }
		virtual const IndexBuffer* GetIndexBuffer() const override { return m_IndexBuffer; };

		virtual void Bind() const override;
		virtual void Unbind() const override;
	private:
		std::vector<VertexBuffer*> m_VertexBuffers;
		IndexBuffer* m_IndexBuffer;
		uint32_t m_RendererID;
		
	};

}