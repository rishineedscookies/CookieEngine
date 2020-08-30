#include "ckpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Cookie {

	void OpenGLRendererAPI::Clear(const mathfu::vec4& color) const
	{
		glClearColor(color.x, color.y, color.z, color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

}