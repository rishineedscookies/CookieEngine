#pragma once

#include <Cookie.h>

#include <mathfu/vector.h>
#include <vectorial/vec4f.h>

class GreenMat : public Cookie::Material
{
public: 

	GreenMat(const std::string& name, const std::string& shaderName)
	{
		SetName(name);
	}

	virtual void Bind() const override { m_MaterialShader->Bind(); }
	virtual void Unbind() const override { m_MaterialShader->Unbind(); }
	virtual void UploadUniformData() const override
	{
		m_MaterialShader->UploadUniformFloat4("u_Albedo", mathfu::vec4(0, 1, 0, 1));
	}

};
