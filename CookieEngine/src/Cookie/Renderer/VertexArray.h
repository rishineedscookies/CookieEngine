#pragma once

#include <memory>
#include "Cookie/Renderer/Buffer.h"

namespace Cookie {

	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		static VertexArray* Create();

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;
		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

	};


}
