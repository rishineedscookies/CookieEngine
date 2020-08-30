#pragma once

#include <memory>
#include "Cookie/Renderer/Buffer.h"

namespace Cookie {

	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		static VertexArray* Create();

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;
		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

	};


}
