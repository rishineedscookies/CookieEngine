#include <Cookie.h>
#include <mathfu/vector.h>
#include <vectorial/vec4f.h>

class ExampleLayer : public Cookie::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_CameraRotation(mathfu::quat::identity)
	{
		m_VertexArray.reset(Cookie::VertexArray::Create());

		float vertices[7 * 3] = {
			-0.5f, -0.5f, 0.0f, 0.2f, 0.4f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f,  0.0f, 0.2f, 0.2f, 1.0f,
			0.0f, 0.5f, 0.0f,   0.4f, 0.0f, 0.4f, 1.0f
		};

		m_VertexBuffer.reset(Cookie::VertexBuffer::Create(vertices, sizeof(vertices)));

		Cookie::BufferLayout layout = {
			{ Cookie::ShaderDataType::Float3, "a_Position" },
			{ Cookie::ShaderDataType::Float4, "a_Color" }
		};

		m_VertexBuffer->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(Cookie::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;			
			layout(location = 1) in vec4 a_Color;	

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
				v_Color = a_Color;
			}

		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;			

			uniform mat4 u_ViewProjection;
			
			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = v_Color;
			}

		)";

		m_Shader.reset(Cookie::Shader::Create(vertexSrc, fragmentSrc));
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
			m_CameraPosition -= m_CameraRotation * mathfu::vec3(0, 0, 1) * m_CameraSpeed * time.DeltaTime;
		}
		if (Cookie::Input::GetKeyDown(CK_KEY_X))
		{
			m_CameraPosition -= m_CameraRotation * mathfu::vec3(0, 0, 1) * m_CameraSpeed * time.DeltaTime;
		}

		Cookie::RenderCommand::Clear(mathfu::vec4(0.6f, 0.14f, 0.29f, 1.0f));

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
		Cookie::Renderer::BeginScene(m_Camera);

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


private:

	std::shared_ptr<Cookie::VertexArray> m_VertexArray;
	std::shared_ptr<Cookie::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<Cookie::IndexBuffer> m_IndexBuffer;
	std::shared_ptr<Cookie::Shader> m_Shader;

	Cookie::OrthographicCamera m_Camera;
	mathfu::vec3 m_CameraPosition;
	float m_CameraSpeed = 1.0f;
	mathfu::quat m_CameraRotation;
	float m_CameraRotSpeed = 3.0f;

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