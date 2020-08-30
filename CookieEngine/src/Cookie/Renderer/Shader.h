#pragma once

#include <string>

#include <mathfu/matrix.h>
#include <mathfu/glsl_mappings.h>

namespace Cookie {

	class Shader
	{
	public:
		virtual ~Shader() {}

		static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void UploadUniformMat4(const std::string& name, const mathfu::mat4& matrix) const = 0;

	};

}