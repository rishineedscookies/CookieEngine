#pragma once

#include <string>

#include <mathfu/vector.h>
#include <mathfu/matrix.h>
#include <mathfu/glsl_mappings.h>

namespace Cookie {

	class Shader
	{
	public:
		virtual ~Shader() {}

		static Ref<Shader> Create(const std::string& path);
		static Ref<Shader> Create(const std::string& path, const std::string& name);
		static Ref<Shader> Create(const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& name);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::string& GetName() const { return m_Name; }


		virtual void UploadUniformBool(const std::string& name, const bool value) const = 0;
		virtual void UploadUniformInt(const std::string& name, const int value) const = 0;
		virtual void UploadUniformIntArray(const std::string& name, const int* values, uint32_t count) const = 0;
		virtual void UploadUniformFloat(const std::string& name, const float value) const = 0;
		virtual void UploadUniformFloat2(const std::string& name, const mathfu::vec2& value) const = 0;
		virtual void UploadUniformFloat3(const std::string& name, const mathfu::vec3& value) const = 0;
		virtual void UploadUniformFloat4(const std::string& name, const mathfu::vec4& value) const = 0;
		virtual void UploadUniformMat2(const std::string& name, const mathfu::mat2& value) const = 0;
		virtual void UploadUniformMat3(const std::string& name, const mathfu::mat3& value) const = 0;
		virtual void UploadUniformMat4(const std::string& name, const mathfu::mat4& value) const = 0;

	protected:

		void SetName(std::string& name) { m_Name = name; }

	protected:
		
		std::string m_Name;
	};

	class ShaderManager
	{
	public:

		void Add(const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader, const std::string& name);
		Ref<Shader> Load(const std::string& path);
		Ref<Shader> Load(const std::string& path, const std::string& name);

		Ref<Shader> Get(const std::string& name);
		bool Exists(const std::string& name);

	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};

}