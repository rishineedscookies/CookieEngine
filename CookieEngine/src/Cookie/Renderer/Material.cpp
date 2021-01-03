#include "ckpch.h"

#include "Cookie/Renderer/Material.h"

namespace Cookie {

	Material::Material(const std::string& name, const std::string& shaderName, bool bInstanced)
	: m_Props() 
	{
		m_Name = name;
		m_ShaderName = shaderName;
		this->bInstanced = bInstanced;
	}

	void Material::AddIntProperty(const char* name, int32_t value)
	{
		m_Props.Ints.Insert(MatProps::MaterialProperty<int32_t>(name, value), m_Props.NumInts++);
	}

	void Material::AddFloatProperty(const char* name, float value)
	{
		m_Props.Floats.Insert(MatProps::MaterialProperty<float>(name, value), m_Props.NumFloats++);
	}

	void Material::AddFloat2Property(const char* name, mathfu::vec2 value)
	{

		m_Props.Float2s.Insert(MatProps::MaterialProperty<mathfu::vec2>(name, value), m_Props.NumFloat2s++);
	}

	void Material::AddFloat3Property(const char* name, mathfu::vec3 value)
	{

		m_Props.Float3s.Insert(MatProps::MaterialProperty<mathfu::vec3>(name, value), m_Props.NumFloat3s++);
	}

	void Material::AddFloat4Property(const char* name, mathfu::vec4 value)
	{
		m_Props.Float4s.Insert(MatProps::MaterialProperty<mathfu::vec4>(name, value), m_Props.NumFloat4s++);
	}

	void Material::AddTextureProperty(const char* name, uint32_t value)
	{
		// TODO
	}

	void Material::UploadUniformData() const
	{	
		for (uint32_t i = 0; i < m_Props.NumInts; i++)
		{
			m_MaterialShader->UploadUniformInt(m_Props.Ints[i].Name, m_Props.Ints[i].Value);
		}
		for (uint32_t i = 0; i < m_Props.NumFloats; i++)
		{
			m_MaterialShader->UploadUniformFloat(m_Props.Floats[i].Name, m_Props.Floats[i].Value);
		}
		for (uint32_t i = 0; i < m_Props.NumFloat2s; i++)
		{
			m_MaterialShader->UploadUniformFloat2(m_Props.Float2s[i].Name, m_Props.Float2s[i].Value);
		}
		for (uint32_t i = 0; i < m_Props.NumFloat3s; i++)
		{
			m_MaterialShader->UploadUniformFloat3(m_Props.Float3s[i].Name, m_Props.Float3s[i].Value);
		}
		for (uint32_t i = 0; i < m_Props.NumFloat4s; i++)
		{
			m_MaterialShader->UploadUniformFloat4(m_Props.Float4s[i].Name, m_Props.Float4s[i].Value);
		}
		// TODO: Textures
	}


	void MaterialManager::Add(const Ref<Material> material, const std::string& name)
	{
		CK_CORE_ASSERT(!Exists(name), "Material already added!");
		m_Materials[name] = material;
	}

	Ref<Material> MaterialManager::Get(const std::string& name)
	{
		CK_CORE_ASSERT(Exists(name), "Material not found!");
		return m_Materials[name];
	}

	bool MaterialManager::Exists(const std::string& name)
	{
		return m_Materials.find(name) != m_Materials.end();
	}


}