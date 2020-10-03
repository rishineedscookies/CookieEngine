#pragma once

#include "Cookie/Renderer/OrthographicCamera.h"
#include "Cookie/Renderer/VertexArray.h"
#include "Cookie/Renderer/Buffer.h"
#include "Cookie/Renderer/Shader.h"
#include "Cookie/Renderer/Material.h"
#include "Cookie/Renderer/Texture.h"
#include <mathfu/vector.h>
#include <mathfu/glsl_mappings.h>

namespace Cookie {

	struct QuadVertex
	{
		mathfu::vec3 Position;
	};

	struct MaterialData
	{
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		uint32_t QuadIndexCount = 0;
		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Material> QuadMaterial;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;
	};

	struct Renderer2DData
	{
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> QuadShader;
		Ref<Shader> TextureShader;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;	

		mathfu::vec4 QuadVertexPositions[4];

		mathfu::mat4 ViewProjectionMatrix;

		std::array<Ref<Material>, 5> Materials;
		std::vector<MaterialData*> MaterialDataList;
	};

	class Renderer2D
	{
	public:
		
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void Flush();
		static void FlushAndReset();
		static void FlushMaterial(MaterialData* data);
		
		static MaterialData* InsertMaterial(Ref<Material> material);
		static MaterialData* CreateMaterialData(Ref<Material> material);

		// Primitives
		static void DrawQuad(const mathfu::mat4& transform, const mathfu::vec4& albedo);
		static void DrawQuad(const mathfu::mat4& transform, const Ref<Material> material);
		static void DrawQuad(const mathfu::vec3& position, const mathfu::quat& rotation, const mathfu::vec2& size, const Ref<Material> material);
		static void DrawQuad(const mathfu::vec3& position, const mathfu::quat& rotation, const mathfu::vec2& size, const mathfu::vec4& albedo);
		static void DrawQuad(const mathfu::mat4& transform, const Ref<Texture2D>& texture, float tillingFactor, const mathfu::vec4& albedo);
		static void DrawQuad(const mathfu::vec3& position, const mathfu::quat& rotation, const mathfu::vec2& size, const Ref<Texture2D>& texture, float tillingFactor, const mathfu::vec4& albedo);

	};

}
