#include "ckpch.h"
#include "Shader.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Cookie {


	Ref<Shader> Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& name)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			CK_CORE_ASSERT(false, "RendererAPI::None is not currently supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLShader>(vertexSrc, fragmentSrc, name);
		default:
			return nullptr;
		}
	}

	Ref<Shader> Shader::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			CK_CORE_ASSERT(false, "RendererAPI::None is not currently supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLShader>(path);
		default:
			return nullptr;
		}

	}

	Ref<Shader> Shader::Create(const std::string& path, const std::string& name)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			CK_CORE_ASSERT(false, "RendererAPI::None is not currently supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLShader>(path, name);
		default:
			return nullptr;
		}

	}

	void ShaderManager::Add(const Ref<Shader>& shader)
	{
		Add(shader, shader->GetName());
	}

	void ShaderManager::Add(const Ref<Shader>& shader, const std::string& name)
	{
		CK_CORE_ASSERT(m_Shaders.find(name) == m_Shaders.end(), "Shader already exists in manager!");
		m_Shaders[name] = shader;
	}

	Ref<Shader> ShaderManager::Load(const std::string& path)
	{
		auto shader = Shader::Create(path);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderManager::Load(const std::string& path, const std::string& name)
	{
		auto shader = Shader::Create(path);
		Add(shader, name);
		return shader;

	}

	Ref<Shader> ShaderManager::Get(const std::string& name)
	{
		CK_CORE_ASSERT(m_Shaders.find(name) != m_Shaders.end(), "Shader not found!");
		return m_Shaders[name];
	}

	bool ShaderManager::Exists(const std::string& name)
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}

}