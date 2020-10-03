#pragma once

#include "Cookie/Renderer/Shader.h"

namespace Cookie {

	

	class Material
	{
	public: 
		Material() {}
		Material(const std::string& name, const std::string& shaderName);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		inline Ref<Shader> GetShader() { return m_MaterialShader; }
		inline void SetShader(Ref<Shader> shader) { m_MaterialShader = shader; }

		inline std::string& GetName() { return m_Name; }
		inline void SetName(const std::string& name) { m_Name = name; }

		virtual void UploadUniformData() const = 0;

	private:
		std::string m_Name;

	protected:
		std::string m_ShaderName;
		Ref<Shader> m_MaterialShader;
	};

	class MaterialManager
	{
	public:
		void Add(const Ref<Material> material, const std::string& name);

		Ref<Material> Get(const std::string& name);
		bool Exists(const std::string& name);

	private:
		std::unordered_map<std::string, Ref<Material>> m_Materials;
	};

}
