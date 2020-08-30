#include "ckpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Cookie {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		CK_CORE_ASSERT(windowHandle, "Window handle is NULL!")
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CK_CORE_ASSERT(status, "Failed to initialize Glad!");

		CK_CORE_INFO("OpenGL Info:");
		CK_CORE_INFO("	OpenGL Renderer: {0}", glGetString(GL_RENDERER));
		CK_CORE_INFO("	OpenGL Vendor: {0}", glGetString(GL_VENDOR));
		CK_CORE_INFO("	OpenGL Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}