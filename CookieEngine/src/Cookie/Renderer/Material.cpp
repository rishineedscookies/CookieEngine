#include "ckpch.h"

#include "Cookie/Renderer/Material.h"

namespace Cookie {

	Material::Material(const std::string& name, const std::string& shaderName)
	{
		m_Name = name;
		m_ShaderName = shaderName;
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