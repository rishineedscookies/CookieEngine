#include "ckpch.h"

#include "Renderer2D.h"

#include "RenderCommand.h"

namespace Cookie {

	static Renderer2DData* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DData();
		s_Data->QuadVertexArray = VertexArray::Create();

		s_Data->QuadVertexBuffer = VertexBuffer::Create(nullptr, s_Data->MaxVertices * sizeof(QuadVertex));
		s_Data->QuadVertexBuffer->SetLayout(
			{
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" },
				{ ShaderDataType::Float2, "a_TexCoord" },
				{ ShaderDataType::Float, "a_TexIndex" },
				{ ShaderDataType::Float, "a_TillingFactor" }
			});
		s_Data->QuadVertexArray->AddVertexBuffer(s_Data->QuadVertexBuffer);

		s_Data->QuadVertexBufferBase = new QuadVertex[s_Data->MaxVertices];
		uint32_t* quadIndices = new uint32_t[s_Data->MaxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data->MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		IndexBuffer* quadIB = IndexBuffer::Create(quadIndices, s_Data->MaxIndices);
		s_Data->QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		int32_t samplers[s_Data->MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data->MaxTextureSlots; i += 6)
		{
			samplers[i] = i;
		}

		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->UploadUniformIntArray("u_Textures", samplers, s_Data->MaxTextureSlots);
		s_Data->QuadShader = Shader::Create("assets/shaders/FlatColor.glsl");

		s_Data->QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data->QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data->QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data->QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		delete[] s_Data->QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		//s_Data->QuadShader->Bind();
		s_Data->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		//s_Data->QuadShader->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data->QuadIndexCount = 0;
		s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;
		s_Data->TextureSlotIndex = 0;
		for (uint32_t i = 0; i < s_Data->MaterialDataList.size(); i++)
		{
			s_Data->MaterialDataList[i]->QuadVertexBufferPtr = s_Data->MaterialDataList[i]->QuadVertexBufferBase;
		}

	}

	void Renderer2D::EndScene()
	{
		//uint32_t dataSize = (uint32_t) ((uint8_t*)s_Data->QuadVertexBufferPtr - (uint8_t*)s_Data->QuadVertexBufferBase);
		//s_Data->QuadVertexBuffer->SetData(s_Data->QuadVertexBufferBase, dataSize);

		//Flush();
		for (uint32_t i = 0; i < s_Data->MaterialDataList.size(); i++)
		{
			FlushMaterial(s_Data->MaterialDataList[i]);
		}
	}

	void Renderer2D::Flush()
	{
		if (s_Data->QuadIndexCount == 0)
		{
			return;
		}

		for (uint32_t i = 0; i < s_Data->TextureSlotIndex; i++)
		{
			s_Data->TextureSlots[i]->Bind(i);
		}

		RenderCommand::DrawIndexed(s_Data->QuadVertexArray, s_Data->QuadIndexCount);
		
	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();

		s_Data->QuadIndexCount = 0;
		s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;
		s_Data->TextureShader = 0;
	}

	void Renderer2D::FlushMaterial(MaterialData* data)
	{
		if (data->QuadIndexCount == 0)
		{
			return;
		}
		data->QuadMaterial->Bind();
		data->QuadMaterial->GetShader()->UploadUniformMat4("u_ViewProjection", s_Data->ViewProjectionMatrix);
		data->QuadMaterial->UploadUniformData();

		uint32_t dataSize = (uint32_t) ((uint8_t*)data->QuadVertexBufferPtr - (uint8_t*)data->QuadVertexBufferBase);
		data->QuadVertexArray->GetIndexBuffer()->Bind();
		data->QuadVertexBuffer->Bind();
		data->QuadVertexBuffer->SetData(data->QuadVertexBufferBase, dataSize);
		data->QuadVertexArray->Bind();
		
		s_Data->TextureSlotIndex = 0;
		for (uint32_t i = 0; i < s_Data->TextureSlotIndex; i++)
		{
			s_Data->TextureSlots[i]->Bind(i);
		}

		RenderCommand::DrawIndexed(data->QuadVertexArray, data->QuadIndexCount);
	}

	MaterialData* Renderer2D::InsertMaterial(Ref<Material> material)
	{
		for (int i = 0; i < s_Data->MaterialDataList.size(); i++)
		{
			Ref<Material> item = s_Data->MaterialDataList[i]->QuadMaterial;
			if (item == material)
			{
				return s_Data->MaterialDataList[i];
			}
		}
		s_Data->MaterialDataList.push_back(CreateMaterialData(material));
		return s_Data->MaterialDataList[s_Data->MaterialDataList.size() - 1];
	}

	MaterialData* Renderer2D::CreateMaterialData(Ref<Material> material)
	{
		MaterialData* data = new MaterialData();
		data->QuadMaterial = material;
		data->QuadVertexArray = VertexArray::Create();

		data->QuadVertexBuffer = VertexBuffer::Create(nullptr, data->MaxVertices * sizeof(QuadVertex));
		data->QuadVertexBuffer->SetLayout(
			{
				{ ShaderDataType::Float3, "a_Position" }
				//{ ShaderDataType::Float4, "a_Color" },
				//{ ShaderDataType::Float2, "a_TexCoord" },
				//{ ShaderDataType::Float, "a_TexIndex" },
			//	{ ShaderDataType::Float, "a_TilingFactor" },
			});

		data->QuadVertexArray->AddVertexBuffer(data->QuadVertexBuffer);

		data->QuadVertexBufferBase = new QuadVertex[data->MaxVertices];
		data->QuadVertexBufferPtr = data->QuadVertexBufferBase;
		uint32_t* quadIndices = new uint32_t[data->MaxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < data->MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		IndexBuffer* quadIB = IndexBuffer::Create(quadIndices, data->MaxIndices);
		data->QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;
		return data;
	}

	void Renderer2D::DrawQuad(const mathfu::mat4& transform, const mathfu::vec4& albedo)
	{
		constexpr size_t quadVertexCount = 4;
		const mathfu::vec2 textureCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f} };


		s_Data->QuadShader->Bind();
		s_Data->QuadShader->UploadUniformFloat4("u_Albedo", albedo); 

		s_Data->QuadShader->UploadUniformMat4("u_Transform", transform);
		s_Data->QuadVertexArray->Bind();

		if (s_Data->QuadIndexCount >= s_Data->MaxIndices)
		{
			FlushAndReset();
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data->QuadVertexBufferPtr->Position = (transform * s_Data->QuadVertexPositions[i]).xyz();
			/*s_Data->QuadVertexBufferPtr->Color = albedo;
			s_Data->QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data->QuadVertexBufferPtr->TexIndex = 0.0;
			s_Data->QuadVertexBufferPtr->TilingFactor = 1.0f;*/
			s_Data->QuadVertexBufferPtr++;
		}

		s_Data->QuadIndexCount += 6;
	}

	void Renderer2D::DrawQuad(const mathfu::vec3& position, const mathfu::quat& rotation, const mathfu::vec2& size, const mathfu::vec4& albedo)
	{
		mathfu::mat4 transformMat = mathfu::mat4::Transform(position, rotation.ToMatrix(), { size.x, size.y, 1 });
		DrawQuad(transformMat, albedo);
	}

	void Renderer2D::DrawQuad(const mathfu::mat4& transform, const Ref<Texture2D>& texture, float tillingFactor, const mathfu::vec4& albedo)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->UploadUniformFloat4("u_Albedo", albedo);

		s_Data->TextureShader->UploadUniformMat4("u_Transform", transform);
		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const mathfu::vec3& position, const mathfu::quat& rotation, const mathfu::vec2& size, const Ref<Texture2D>& texture, float tillingFactor, const mathfu::vec4& albedo)
	{
		mathfu::mat4 transformMat = mathfu::mat4::Transform(position, rotation.ToMatrix(), { size.x, size.y, 0 });
		DrawQuad(transformMat, texture, tillingFactor, albedo);
	}

	void Renderer2D::DrawQuad(const mathfu::mat4& transform, const Ref<Material> material)
	{
		constexpr size_t quadVertexCount = 4;
		const mathfu::vec2 textureCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f} };

		MaterialData* data = InsertMaterial(material);

		if (data->QuadIndexCount >= data->MaxIndices)
		{
			FlushAndReset();
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			data->QuadVertexBufferPtr->Position = (transform * s_Data->QuadVertexPositions[i]).xyz();
			//data->QuadVertexBufferPtr->Color = mathfu::vec4(1);
			//data->QuadVertexBufferPtr->TexCoord = textureCoords[i];
			//data->QuadVertexBufferPtr->TexIndex = 0;
			//data->QuadVertexBufferPtr->TilingFactor = 1.0f;
			data->QuadVertexBufferPtr++;
		}

		data->QuadIndexCount += 6;
		
	}

	void Renderer2D::DrawQuad(const mathfu::vec3& position, const mathfu::quat& rotation, const mathfu::vec2& size, const Ref<Material> material)
	{
		mathfu::mat4 transformMat = mathfu::mat4::Transform(position, rotation.ToMatrix(), { size.x, size.y, 1 });
		DrawQuad(transformMat, material);
	}

}