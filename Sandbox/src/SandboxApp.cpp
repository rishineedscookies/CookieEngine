#include <Cookie.h>
#include <Cookie/Core/EntryPoint.h>

#include <mathfu/vector.h>
#include <vectorial/vec4f.h>

#include <imgui/imgui.h>
#include "GreenMat.h"
#include "RedMat.h"

#include "PositionComponent.h"
#include "AABBColliderComponent.h"
#include "MovementComponent.h"
#include "TransformComponent.h"
#include "MeshRenderComponent.h"
#include "FirstPersonCameraComponent.h"
#include "PhysicsSystem.h"
#include "MovementSystem.h"
#include "InputSystem.h"
#include "FirstPersonCameraSystem.h"

class ExampleLayer : public Cookie::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f, 0.0f, 10.0f), m_CameraRotation(mathfu::quat::identity)
	{
		m_ShaderManager.Load("assets/shaders/Texture.glsl");
		m_ShaderManager.Load("assets/shaders/FlatColor.glsl");
		m_ShaderManager.Load("assets/shaders/SimpleDiffuse.glsl", "Mesh");
		m_ShaderManager.Load("assets/shaders/SimpleBlinnPhong.glsl", "BlinnPhong");
		m_TextureShader = Cookie::Shader::Create("assets/shaders/Texture.glsl");

		m_Texture = Cookie::Texture2D::Create("assets/textures/HealthCross.png");
		m_TextureShader->Bind();
		m_TextureShader->UploadUniformInt("u_Texture", 0);

		m_GreenMaterial = Cookie::CreateRef<GreenMat>("GreenMat", "BlinnPhong");
		m_GreenMaterial->SetShader(m_ShaderManager.Get("BlinnPhong"));
		m_RedMaterial = Cookie::CreateRef<RedMat>("RedMat", "Mesh");
		m_RedMaterial->SetShader(m_ShaderManager.Get("Mesh"));
		m_MaterialManager.Add(m_GreenMaterial, "GreenMat");
		m_MaterialManager.Add(m_RedMaterial, "RedMat");

		m_CubeModel = m_ModelManager.LoadModel("assets/models/objcube.obj");
		m_Monkey = m_ModelManager.LoadModel("assets/models/monkey.obj");

		m_World = new Cookie::World();

		m_World->RegisterComponent<PositionComponent>(POSITION_ID);
		m_World->RegisterComponent<AABBColliderComponent>(AABB_COLLIDER_ID);
		m_World->RegisterComponent<MovementComponent>(MOVEMENT_ID);
		m_World->RegisterComponent<TransformComponent>(TRANSFORM_ID);
		m_World->RegisterComponent<MeshRenderComponent>(MESH_RENDER_ID);
		m_World->RegisterComponent<FirstPersonCameraComponent>(FIRST_PERSON_CAMERA_ID);

		m_PhysicsSystem = new PhysicsSystem();
		m_MovementSystem = new MovementSystem();
		m_FirstPersonCameraSystem = new FirstPersonCameraSystem();

		{
			Cookie::Entity Cube = m_World->AddEntity();
			m_World->AddComponent<TransformComponent>(Cube, TRANSFORM_ID);
			MeshRenderComponent* cubeModel = m_World->AddComponent<MeshRenderComponent>(Cube, MESH_RENDER_ID);
			cubeModel->Model = m_Monkey;
			cubeModel->Material = &*m_GreenMaterial;
		}

		{
			m_Player = m_World->AddEntity();

			//MeshRenderComponent* mesh = m_World->AddComponent<MeshRenderComponent>(m_Player, MESH_RENDER_ID);
			//mesh->Model = m_CubeModel;
			//mesh->Material = &*m_RedMaterial;
		
			m_World->AddComponent<MovementComponent>(m_Player, MOVEMENT_ID);
			TransformComponent* PlayerTransform = m_World->AddComponent<TransformComponent>(m_Player, TRANSFORM_ID);
			PlayerTransform->Transform = mathfu::mat4::Transform(mathfu::vec3(5.0f, 2.5f, 5.0f),
				mathfu::mat3::Identity(),
				mathfu::vec3(1.0f));

			AABBColliderComponent* PlayerCollider = m_World->AddComponent<AABBColliderComponent>(m_Player, AABB_COLLIDER_ID);
			PlayerCollider->Offset = mathfu::vec3(0.0f);
			PlayerCollider->HalfExtents = mathfu::vec3(1.0f);

			m_FirstPersonCamera = m_World->AddComponent<FirstPersonCameraComponent>(m_Player, FIRST_PERSON_CAMERA_ID);
			*m_FirstPersonCamera = FirstPersonCameraComponent(-1.6f, 1.6f, -0.9f, 0.9f);
		}


		for (uint32_t floorX = 0; floorX < 10; floorX++)
		{
			for (uint32_t floorY = 0; floorY < 10; floorY++)
			{
				m_Floor = m_World->AddEntity();
				MeshRenderComponent* mesh = m_World->AddComponent<MeshRenderComponent>(m_Floor, MESH_RENDER_ID);
				mesh->Model = m_CubeModel;
				mesh->Material = &*m_GreenMaterial;
				m_World->AddComponent<TransformComponent>(m_Floor, TRANSFORM_ID)->Transform = mathfu::mat4::Transform(mathfu::vec3(floorX, 0, floorY),
					mathfu::mat3::Identity(),
					mathfu::vec3(1.0f));
				AABBColliderComponent* FloorCollider = m_World->AddComponent<AABBColliderComponent>(m_Floor, AABB_COLLIDER_ID);
				FloorCollider->Offset = mathfu::vec3(0.0f);
				FloorCollider->HalfExtents = mathfu::vec3(1.0f);
			}
		}
	}

	void OnUpdate(Cookie::Time time) override
	{
		InputSystem::OnUpdate(m_World, &time);
		CK_TRACE("Mouse Delta: {0}, {1}", InputSystem::DeltaMouseX, InputSystem::DeltaMouseY);
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
		m_FirstPersonCameraSystem->OnUpdate(m_World, &time);
		m_MovementSystem->OnUpdate(m_World, &time);
		m_PhysicsSystem->OnUpdate(m_World, &time);

		Cookie::RenderCommand::Clear(mathfu::vec4(0.6f, 0.14f, 0.29f, 1.0f));

		//m_Camera.SetPosition(m_CameraPosition);
		//m_Camera.SetRotation(m_CameraRotation);
		Cookie::PointLight light = Cookie::PointLight(mathfu::vec3(3.0f), mathfu::vec3(0.0f, 0.0f, 0.0f));
		Cookie::DirectionalLight dirLight = Cookie::DirectionalLight(mathfu::vec3(0.2f, -1.0f, -0.3f), mathfu::vec3(0.5f, 0.5f, 0.6f));
		Cookie::Renderer::BeginScene(m_FirstPersonCamera->Camera, &light, &dirLight);
		Cookie::Renderer2D::BeginScene(m_FirstPersonCamera->Camera);

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

	void RenderOnUpdate(Cookie::Time Time)
	{
		Cookie::ComponentManager<TransformComponent>* Transforms = m_World->GetComponentManager<TransformComponent>(TRANSFORM_ID);
		Cookie::ComponentManager<MeshRenderComponent>* Meshes = GET_POOL(m_World, MeshRenderComponent, MESH_RENDER_ID);
		for (Cookie::size_t i = 0; i < Meshes->Size; i++)
		{
			Cookie::Entity e = (*Meshes->DenseEntities)[i];

			if (e == INVALID_ENTITY)
				continue;

			MeshRenderComponent* MeshRender = GET_COMPONENT(Meshes, e);
			TransformComponent* Transform = GET_COMPONENT(Transforms, e);

			Cookie::Renderer::DrawModel(MeshRender->Model, MeshRender->Material, &Transform->Transform);
			//Cookie::Renderer2D::DrawQuad(Transform->Transform, m_GreenMaterial);
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

	Cookie::ModelManager m_ModelManager;
	Cookie::Model* m_CubeModel;
	Cookie::Model* m_Monkey;

	FirstPersonCameraComponent* m_FirstPersonCamera;
	Cookie::OrthographicCamera m_Camera;
	mathfu::vec3 m_CameraPosition;
	float m_CameraSpeed = 10.0f;
	mathfu::quat m_CameraRotation;
	float m_CameraRotSpeed = 1.0f;
	float m_ZoomLevel = 1.0f;
	
	Cookie::Entity m_Player;
	PhysicsSystem* m_PhysicsSystem;
	MovementSystem* m_MovementSystem;
	FirstPersonCameraSystem* m_FirstPersonCameraSystem;

	Cookie::Entity m_Floor;
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