#include "ckpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture2D.h"

namespace Cookie {

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
			switch (Renderer::GetAPI())
			{
			case RendererAPI::API::None:
				CK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLTexture2D>(path);
			default:
				return nullptr;
			}
	}

}