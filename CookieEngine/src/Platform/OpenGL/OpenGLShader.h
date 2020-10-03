#pragma once

#include "Cookie/Renderer/Shader.h"

#include <glad/glad.h>

namespace Cookie {

	class OpenGLShader : public Shader
	{
	public:

		OpenGLShader(const std::string& path);
		OpenGLShader(const std::string& path, const std::string& name);
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& name);
		~OpenGLShader();

		void Bind() const override;
		void Unbind() const override;

		virtual void UploadUniformBool(const std::string& name, const bool value) const override;
		virtual void UploadUniformInt(const std::string& name, const int value) const override;
		virtual void UploadUniformIntArray(const std::string& name, const int* values, uint32_t count) const override;
		virtual void UploadUniformFloat(const std::string& name, const float value) const override;
		virtual void UploadUniformFloat2(const std::string& name, const mathfu::vec2& value) const override;
		virtual void UploadUniformFloat3(const std::string& name, const mathfu::vec3& value) const override;
		virtual void UploadUniformFloat4(const std::string& name, const mathfu::vec4& value) const override;
		virtual void UploadUniformMat2(const std::string& name, const mathfu::mat2& value) const override;
		virtual void UploadUniformMat3(const std::string& name, const mathfu::mat3& value) const override;
		virtual void UploadUniformMat4(const std::string& name, const mathfu::mat4& value) const override;

	private:
		std::string ReadFile(const std::string& path);
		std::unordered_map<GLenum, std::string> Preproccess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string> sources);
	private:

		uint32_t m_RendererID;

	};

}