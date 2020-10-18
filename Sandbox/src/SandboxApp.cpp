#include <Cookie.h>

#include <mathfu/vector.h>
#include <vectorial/vec4f.h>

#include <imgui/imgui.h>
#include "GreenMat.h"
#include "RedMat.h"

#include "PositionComponent.h"

class ExampleLayer : public Cookie::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_CameraRotation(mathfu::quat::identity)
	{
		m_World = new Cookie::World();

		m_World->RegisterComponent<PositionComponent>(POSITION_ID);
	
		for (int i = 0; i < 10; i++)
		{
			Cookie::Entity entity = m_World->AddEntity();
			PositionComponent* position = m_World->AddComponent<PositionComponent>(entity, POSITION_ID);
			position->Position.y = i;
		}

		m_ShaderManager.Load("assets/shaders/Texture.glsl");
		m_ShaderManager.Load("assets/shaders/FlatColor.glsl");
		m_TextureShader = Cookie::Shader::Create("assets/shaders/Texture.glsl");

		m_Texture = Cookie::Texture2D::Create("assets/textures/HealthCross.png");
		m_TextureShader->Bind();
		m_TextureShader->UploadUniformInt("u_Texture", 0);

		m_GreenMaterial = Cookie::CreateRef<GreenMat>("GreenMat", "FlatColor");
		m_GreenMaterial->SetShader(m_ShaderManager.Get("FlatColor"));
		m_RedMaterial = Cookie::CreateRef<RedMat>("RedMat", "FlatColor");
		m_RedMaterial->SetShader(m_ShaderManager.Get("FlatColor"));
		m_MaterialManager.Add(m_GreenMaterial, "GreenMat");
		m_MaterialManager.Add(m_RedMaterial, "RedMat");
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
		MovementOnUpdate(time);

		Cookie::RenderCommand::Clear(mathfu::vec4(0.6f, 0.14f, 0.29f, 1.0f));

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
		Cookie::Renderer::BeginScene(m_Camera);
		Cookie::Renderer2D::BeginScene(m_Camera);

		RenderOnUpdate(time);
	
		Cookie::Renderer2D::EndScene();
		Cookie::Renderer::EndScene();
	}

	void OnEvent(Cookie::Event& event) override
	{
		Cookie::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Cookie::MouseScrolledEvent>(CK_BIND_EVENT_FN(ExampleLayer::OnMouseScrollEvent));
	}

	bool OnMouseScrollEvent(Cookie::MouseScrolledEvent e)
	{
		float scrollSensitivity = 0.1f;
		const float aspectRatio = 1.7778f;
		m_ZoomLevel -= e.GetYOffset() * scrollSensitivity;
		const float left = aspectRatio * -m_ZoomLevel;
		const float right = aspectRatio * m_ZoomLevel;
		const float bottom = -m_ZoomLevel;
		const float up = m_ZoomLevel;
		m_Camera.SetProjectionMatrix(left, right, bottom, up);
		return false;
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::End();
	}

	void MovementOnUpdate(Cookie::Time Time)
	{
		std::vector<PositionComponent>* Positions = m_World->GetComponentManager<PositionComponent>(POSITION_ID)->ComponentPool;
		for (int i = 0; i < Positions->size(); i++)
		{
			(*Positions)[i].Position += mathfu::vec3(1.0f, 0.0f, 0.0f) * Time.DeltaTime;
		}
	}

	void RenderOnUpdate(Cookie::Time Time)
	{
		std::vector<PositionComponent>* Positions = m_World->GetComponentManager<PositionComponent>(POSITION_ID)->ComponentPool;
		for (int i = 0; i < Positions->size(); i++)
		{
			PositionComponent* Position = &(*Positions)[i];
			mathfu::mat4 transform = mathfu::mat4::Transform(Position->Position, mathfu::quat::identity.ToMatrix(), mathfu::vec3(1));
			Cookie::Renderer2D::DrawQuad(transform, m_GreenMaterial);
		}
	}


private:

	Cookie::World* m_World;

	Cookie::ShaderManager m_ShaderManager;
	Cookie::Ref<Cookie::Shader> m_Shader;
	Cookie::Ref<Cookie::Shader> m_TextureShader;

	Cookie::Ref<Cookie::Texture2D> m_Texture;

	Cookie::MaterialManager m_MaterialManager;
	Cookie::Ref<Cookie::Material> m_GreenMaterial;
	Cookie::Ref<Cookie::Material> m_RedMaterial;

	Cookie::OrthographicCamera m_Camera;
	mathfu::vec3 m_CameraPosition;
	float m_CameraSpeed = 100.0f;
	mathfu::quat m_CameraRotation;
	float m_CameraRotSpeed = 3.0f;
	float m_ZoomLevel = 1.0f;
	
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