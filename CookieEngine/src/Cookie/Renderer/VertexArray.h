#pragma once

#include <memory>
#include "Cookie/Renderer/Buffer.h"

namespace Cookie {

	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		static VertexArray* Create();

		virtual void AddVertexBuffer(const VertexBuffer* vertexBuffer) = 0;
		virtual void SetIndexBuffer(const IndexBuffer* indexBuffer) = 0;
		virtual const std::vector<VertexBuffer*>& GetVertexBuffers() const = 0;
		virtual const IndexBuffer* GetIndexBuffer() const = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

	};


}
