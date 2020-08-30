#pragma once

#include "Cookie/Renderer/Shader.h"

namespace Cookie {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader();

		void Bind() const override;
		void Unbind() const override;

		virtual void UploadUniformMat4(const std::string& name, const mathfu::mat4& matrix) const override;


	private:

		uint32_t m_RendererID;

	};

}