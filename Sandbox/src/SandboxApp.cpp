#include <Cookie.h>
#include <mathfu/vector.h>
#include <vectorial/vec4f.h>

#include <imgui/imgui.h>

class ExampleLayer : public Cookie::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_CameraRotation(mathfu::quat::identity), m_SquarePos(0.0f)
	{
		m_VertexArray.reset(Cookie::VertexArray::Create());
		m_SqVertexArray.reset(Cookie::VertexArray::Create());

		float vertices[3 * 3] =
		{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
		};


		m_VertexBuffer.reset(Cookie::VertexBuffer::Create(vertices, sizeof(vertices)));

		Cookie::BufferLayout layout = {
			{ Cookie::ShaderDataType::Float3, "a_Position" }
		};

		m_VertexBuffer->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(Cookie::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		float squareVerts[4 * 5] =
		{
			-0.75f, 0.75f, 0.0f,	0.0f, 1.0f,
			-0.75f, -0.75f, 0.0f,	0.0f, 0.0f,
			0.75f, -0.75f, 0.0f,	1.0f, 0.0f,
			0.75f, 0.75f, 0.0f,		1.0f, 1.0f
		};

		m_SqVertexBuffer.reset(Cookie::VertexBuffer::Create(squareVerts, sizeof(squareVerts)));

		Cookie::BufferLayout sqLayout = {
			{ Cookie::ShaderDataType::Float3, "a_Position" },
			{ Cookie::ShaderDataType::Float2, "a_TexCoord"}
		};

		m_SqVertexBuffer->SetLayout(sqLayout);

		m_SqVertexArray->AddVertexBuffer(m_SqVertexBuffer);

		uint32_t sqIndices[6] = { 0, 1, 2, 0, 2, 3};

		m_SqIndexBuffer.reset(Cookie::IndexBuffer::Create(sqIndices, sizeof(sqIndices) / sizeof(uint32_t)));

		m_SqVertexArray->SetIndexBuffer(m_SqIndexBuffer);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}

		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;			

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			uniform vec4 u_Albedo;			

			in vec3 v_Position;

			void main()
			{
				color = u_Albedo;
			}

		)";

		m_Shader = Cookie::Shader::Create(vertexSrc, fragmentSrc, "AlbedoShader");

		
		m_ShaderManager.Load("assets/shaders/Texture.glsl");
		m_TextureShader = Cookie::Shader::Create("assets/shaders/Texture.glsl");

		m_Texture = Cookie::Texture2D::Create("assets/textures/HealthCross.png");
		m_TextureShader->Bind();
		m_TextureShader->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Cookie::Time time) override
	{
		if (Cookie::Input::GetKeyDown(CK_KEY_Q))
		{
			m_CameraRotation = m_CameraRotation * mathfu::quat::FromAngleAxis(m_CameraRotSpeed * time.DeltaTime, mathfu::vec3(0, 0, 1));
		}
		if (Cookie::Input::GetKeyDown(CK_KEY_E))
		{																																
			m_CameraRotation = m_CameraRotation * mathfu::quat::FromAngleAxis(-m_CameraRotSpeed * time.DeltaTime, mathfu::vec3(0, 0, 1));
		}
		if (Cookie::Input::GetKeyDown(CK_KEY_R))
		{
			m_CameraRotation = m_CameraRotation * mathfu::quat::FromAngleAxis(m_CameraRotSpeed * time.DeltaTime, mathfu::vec3(0, 1, 0));
		}
		if (Cookie::Input::GetKeyDown(CK_KEY_F))
		{
			m_CameraRotation = m_CameraRotation * mathfu::quat::FromAngleAxis(-m_CameraRotSpeed * time.DeltaTime, mathfu::vec3(0, 1, 0));
		}
		if (Cookie::Input::GetKeyDown(CK_KEY_T))
		{
			m_CameraRotation = m_CameraRotation * mathfu::quat::FromAngleAxis(m_CameraRotSpeed * time.DeltaTime, mathfu::vec3(1, 0, 0));
		}
		if (Cookie::Input::GetKeyDown(CK_KEY_G))
		{
			m_CameraRotation = m_CameraRotation * mathfu::quat::FromAngleAxis(-m_CameraRotSpeed * time.DeltaTime, mathfu::vec3(1, 0, 0));
		}

		if (Cookie::Input::GetKeyDown(CK_KEY_W))
		{
			m_CameraPosition += m_CameraRotation * mathfu::vec3(0, 1, 0) * m_CameraSpeed * time.DeltaTime;
		}
		if (Cookie::Input::GetKeyDown(CK_KEY_A))
		{
			m_CameraPosition -= m_CameraRotation * mathfu::vec3(1, 0, 0) * m_CameraSpeed * time.DeltaTime;
		}
		if (Cookie::Input::GetKeyDown(CK_KEY_S))
		{
			m_CameraPosition -= m_CameraRotation * mathfu::vec3(0, 1, 0) * m_CameraSpeed * time.DeltaTime;
		}
		if (Cookie::Input::GetKeyDown(CK_KEY_D))
		{
			m_CameraPosition += m_CameraRotation * mathfu::vec3(1, 0, 0) * m_CameraSpeed * time.DeltaTime;
		}
		if (Cookie::Input::GetKeyDown(CK_KEY_Z))
		{
			m_CameraPosition += m_CameraRotation * mathfu::vec3(0, 0, 1) * m_CameraSpeed * time.DeltaTime;
		}
		if (Cookie::Input::GetKeyDown(CK_KEY_X))
		{
			m_CameraPosition -= m_CameraRotation * mathfu::vec3(0, 0, 1) * m_CameraSpeed * time.DeltaTime;
		}

		if (Cookie::Input::GetKeyDown(CK_KEY_UP))
		{
			m_SquarePos += mathfu::vec3(0, 1, 0) * m_CameraSpeed * time.DeltaTime;
		}
		if (Cookie::Input::GetKeyDown(CK_KEY_LEFT))
		{
			m_SquarePos -= mathfu::vec3(1, 0, 0) * m_CameraSpeed * time.DeltaTime;
		}
		if (Cookie::Input::GetKeyDown(CK_KEY_DOWN))
		{
			m_SquarePos -= mathfu::vec3(0, 1, 0) * m_CameraSpeed * time.DeltaTime;
		}
		if (Cookie::Input::GetKeyDown(CK_KEY_RIGHT))
		{
			m_SquarePos += mathfu::vec3(1, 0, 0) * m_CameraSpeed * time.DeltaTime;
		}

		Cookie::RenderCommand::Clear(mathfu::vec4(0.6f, 0.14f, 0.29f, 1.0f));

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
		Cookie::Renderer::BeginScene(m_Camera);

		mathfu::mat4 sqTransform = mathfu::mat4::FromTranslationVector(m_SquarePos);

		m_TextureShader->Bind();
		m_TextureShader->UploadUniformFloat4("u_Albedo", m_SquareColor);
		m_Texture->Bind();
		Cookie::Renderer::Submit(m_TextureShader, m_SqVertexArray, sqTransform);

		m_Shader->Bind();
		m_Shader->UploadUniformFloat4("u_Albedo", mathfu::vec4(0.4f, 0.15f, 0.5f, 1.0f));
		Cookie::Renderer::Submit(m_Shader, m_VertexArray);


		Cookie::Renderer::EndScene();
	}

	void OnEvent(Cookie::Event& event) override
	{
		Cookie::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Cookie::KeyPressedEvent>(CK_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(Cookie::KeyPressedEvent event)
	{
		return false;
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Square Color", &(m_SquareColor[0]));
		ImGui::End();
	}


private:
	Cookie::ShaderManager m_ShaderManager;
	Cookie::Ref<Cookie::VertexArray> m_VertexArray;
	Cookie::Ref<Cookie::VertexBuffer> m_VertexBuffer;
	Cookie::Ref<Cookie::IndexBuffer> m_IndexBuffer;
	Cookie::Ref<Cookie::VertexArray> m_SqVertexArray;
	Cookie::Ref<Cookie::VertexBuffer> m_SqVertexBuffer;
	Cookie::Ref<Cookie::IndexBuffer> m_SqIndexBuffer;
	Cookie::Ref<Cookie::Shader> m_Shader;
	Cookie::Ref<Cookie::Shader> m_TextureShader;

	Cookie::Ref<Cookie::Texture2D> m_Texture;

	Cookie::OrthographicCamera m_Camera;
	mathfu::vec3 m_CameraPosition;
	float m_CameraSpeed = 1.0f;
	mathfu::quat m_CameraRotation;
	float m_CameraRotSpeed = 3.0f;

	mathfu::vec3 m_SquarePos;
	mathfu::vec4 m_SquareColor;
};

class Sandbox : public Cookie::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Cookie::Application* Cookie::CreateApplication()
{
	return new Sandbox();
}