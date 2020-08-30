#include "ckpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Cookie {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();

}