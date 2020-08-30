#pragma once
#include "Cookie/Renderer/RenderingContext.h"

struct GLFWwindow;

namespace Cookie {

	class OpenGLContext : public RenderingContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};

}

