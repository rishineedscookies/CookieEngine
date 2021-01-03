#include "ckpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Cookie {

	void OpenGLDebugMessageCallback(unsigned source, unsigned type, unsigned id, unsigned severity, int length, const char* message, const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_NOTIFICATION: CK_CORE_TRACE(message); return;
		case GL_DEBUG_SEVERITY_LOW: CK_CORE_WARN(message); return;
		case GL_DEBUG_SEVERITY_MEDIUM: CK_CORE_ERROR(message); return;
		case GL_DEBUG_SEVERITY_HIGH: CK_CORE_ERROR(message); return;
		}
		CK_CORE_ASSERT(false, "Unknown OpenGL severity level!");
	}

	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLDebugMessageCallback, NULL);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, int32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::Clear(const mathfu::vec4& color) const
	{
		glClearColor(color.x, color.y, color.z, color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const VertexArray* vertexArray, uint32_t count)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::DrawIndexedInstanced(const VertexArray* vertexArray, uint32_t count, uint32_t instances)
	{	
		glDrawElementsInstanced(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, NULL, instances);
	}

}