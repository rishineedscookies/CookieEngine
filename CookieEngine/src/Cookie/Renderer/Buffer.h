#pragma once

namespace Cookie
{

	enum class ShaderDataType : uint8_t
	{
		None = 0, 
		Bool,
		Float, Float2, Float3, Float4,
		Mat2, Mat3, Mat4,
		Int, Int2, Int3, Int4
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::None:   return 0;
		case ShaderDataType::Bool:   return 1;
		case ShaderDataType::Float : return 4;
		case ShaderDataType::Float2: return 4 * 2;
		case ShaderDataType::Float3: return 4 * 3;
		case ShaderDataType::Float4: return 4 * 4;
		case ShaderDataType::Mat2:   return 4 * 2 * 2;
		case ShaderDataType::Mat3:   return 4 * 3 * 3;
		case ShaderDataType::Mat4:   return 4 * 4 * 4;
		case ShaderDataType::Int:    return 4;
		case ShaderDataType::Int2:   return 4 * 2;
		case ShaderDataType::Int3:   return 4 * 3;
		case ShaderDataType::Int4:   return 4 * 4;
		}
		CK_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	static uint32_t GetComponentCount(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::None:	 return 0;
		case ShaderDataType::Bool:	 return 1;
		case ShaderDataType::Float:  return 1;
		case ShaderDataType::Float2: return 2;
		case ShaderDataType::Float3: return 3;
		case ShaderDataType::Float4: return 4;
		case ShaderDataType::Mat2:   return 2 * 2;
		case ShaderDataType::Mat3:   return 3 * 3;
		case ShaderDataType::Mat4:   return 4 * 4;
		case ShaderDataType::Int:    return 1;
		case ShaderDataType::Int2:   return 2;
		case ShaderDataType::Int3:   return 3;
		case ShaderDataType::Int4:   return 4;
		}
		CK_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Offset;
		uint32_t Size;
		bool Normalized;

		BufferElement() {}

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{}
	};

	class BufferLayout
	{
	public:

		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement> elements)
			: m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		inline const uint32_t GetStride() const { return m_Stride; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:

		void CalculateOffsetsAndStride();

	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);

		virtual void SetData(const void* data, uint32_t size) = 0;


		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);

		virtual uint32_t GetCount() const = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};

}

