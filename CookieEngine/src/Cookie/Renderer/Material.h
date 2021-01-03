#pragma once

#include "Cookie/Renderer/Shader.h"
#include "Cookie/Renderer/Texture.h"
#include "Cookie/Common/Vector.h"

namespace Cookie {

	namespace MatProps
	{
		template<typename T>
		struct MaterialProperty
		{
			MaterialProperty()
				: Name(NULL)
			{}

			MaterialProperty(const char* name, T value)
				: Name(name), Value(value)
			{}

			const char* Name;
			T Value;
		};

		struct MaterialProperties
		{
			MaterialProperties()
				: Ints(0),
				NumInts(0),
				Floats(0),
				NumFloats(0),
				Float2s(0),
				NumFloat2s(0),
				Float3s(0),
				NumFloat3s(0),
				Float4s(0),
				NumFloat4s(0),
				Textures(0),
				NumTextures(0)
			{}

			Vector<MaterialProperty<int32_t>> Ints;
			uint32_t NumInts;
			Vector<MaterialProperty<float>> Floats;
			uint32_t NumFloats;
			Vector<MaterialProperty<mathfu::vec2>> Float2s;
			uint32_t NumFloat2s;
			Vector<MaterialProperty<mathfu::vec3>> Float3s;
			uint32_t NumFloat3s;
			Vector<MaterialProperty<mathfu::vec4>> Float4s;
			uint32_t NumFloat4s;
			Vector<MaterialProperty<uint32_t>> Textures;
			uint32_t NumTextures;
		};
	}

	class Material
	{
	public:
		Material() {}
		Material(const std::string& name, const std::string& shaderName, bool bInstanced = false);

		inline void Bind() const { m_MaterialShader->Bind(); }
		inline void Unbind() const { m_MaterialShader->Unbind(); }

		inline Ref<Shader> GetShader() { return m_MaterialShader; }
		inline void SetShader(Ref<Shader> shader) { m_MaterialShader = shader; }

		inline std::string& GetName() { return m_Name; }
		inline void SetName(const std::string& name) { m_Name = name; }

		void AddIntProperty(const char* name, int32_t value);
		void AddFloatProperty(const char* name, float value);
		void AddFloat2Property(const char* name, mathfu::vec2 value);
		void AddFloat3Property(const char* name, mathfu::vec3 value);
		void AddFloat4Property(const char* name, mathfu::vec4 value);
		void AddTextureProperty(const char* name, uint32_t value);


		void UploadUniformData() const;

	private:

	protected:
		std::string m_Name;
		bool bInstanced;
		std::string m_ShaderName;
		Ref<Shader> m_MaterialShader;
		MatProps::MaterialProperties m_Props;
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
