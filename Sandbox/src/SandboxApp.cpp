#include <Cookie.h>
#include <Cookie/Core/EntryPoint.h>

#include <mathfu/vector.h>
#include <vectorial/vec4f.h>

#include <imgui/imgui.h>

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
		m_ShaderManager.Load("assets/shaders/InstancedBlinnPhong.glsl", "InstancedBlinnPhong");
		m_TextureShader = Cookie::Shader::Create("assets/shaders/Texture.glsl");

		m_Texture = Cookie::Texture2D::Create("assets/textures/HealthCross.png");
		m_TextureShader->Bind();
		m_TextureShader->UploadUniformInt("u_Texture", 0);

		m_GreenMaterial = Cookie::CreateRef<Cookie::Material>("GreenMat", "BlinnPhong");
		m_GreenMaterial->SetShader(m_ShaderManager.Get("BlinnPhong"));
		m_GreenMaterial->AddFloat4Property("u_Albedo", mathfu::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		m_InstancedRedMaterial = Cookie::CreateRef<Cookie::Material>("InstancedRedMat", "InstancedBlinnPhong");
		m_InstancedRedMaterial->SetShader(m_ShaderManager.Get("InstancedBlinnPhong"));
		m_InstancedRedMaterial->AddFloat4Property("u_Albedo", mathfu::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		m_RedMaterial = Cookie::CreateRef<Cookie::Material>("RedMat", "BlinnPhong");
		m_RedMaterial->SetShader(m_ShaderManager.Get("BlinnPhong"));
		m_RedMaterial->AddFloat4Property("u_Albedo", mathfu::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		m_MaterialManager.Add(m_GreenMaterial, "GreenMat");
		m_MaterialManager.Add(m_InstancedRedMaterial, "InstancedRedMat");
		m_MaterialManager.Add(m_RedMaterial, "RedMat");

		m_CubeModel = m_ModelManager.LoadInstancedModel("assets/models/objcube.obj");
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

		CK_TRACE("Mouse Delta: {0}, {1}", InputSystem::DeltaMouseX, InputSystem::DeltaMouseY);

		for (uint32_t floorX = 0; floorX < 20; floorX++)
		{
			for (uint32_t floorY = 0; floorY < 20; floorY++)
			{
				m_Floor = m_World->AddEntity();
				MeshRenderComponent* mesh = m_World->AddComponent<MeshRenderComponent>(m_Floor, MESH_RENDER_ID);
				mesh->Model = m_CubeModel;
				mesh->Material = &*m_InstancedRedMaterial;
				m_World->AddComponent<TransformComponent>(m_Floor, TRANSFORM_ID)->Transform = mathfu::mat4::Transform(mathfu::vec3(floorX, 0, floorY),
					mathfu::mat3::Identity(),
					mathfu::vec3(1.0f));
				AABBColliderComponent* FloorCollider = m_World->AddComponent<AABBColliderComponent>(m_Floor, AABB_COLLIDER_ID);
				FloorCollider->Offset = mathfu::vec3(0.0f);
				FloorCollider->HalfExtents = mathfu::vec3(1.0f);
			}
		}

		{
			Cookie::Entity Monkey = m_World->AddEntity();
			m_World->AddComponent<TransformComponent>(Monkey, TRANSFORM_ID)->Transform = mathfu::mat4::Transform(mathfu::vec3(5.0f, 2.5f, 4.0f), mathfu::mat3::Identity(), mathfu::vec3(1.0f));
			MeshRenderComponent* cubeModel = m_World->AddComponent<MeshRenderComponent>(Monkey, MESH_RENDER_ID);
			cubeModel->Model = m_Monkey;
			cubeModel->Material = &*m_GreenMaterial;
		}
	}

	void OnUpdate(Cookie::Time time) override
	{
		PROFILE_SCOPE("SandboxApp::OnUpdate");

		InputSystem::OnUpdate(m_World, &time);
		m_FirstPersonCameraSystem->OnUpdate(m_World, &time);
		m_MovementSystem->OnUpdate(m_World, &time);
		//m_PhysicsSystem->OnUpdate(m_World, &time);

		Cookie::RenderCommand::Clear(mathfu::vec4(0.6f, 0.14f, 0.29f, 1.0f));

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
		ImGui::Text("Profiler");
		for (uint32_t i = 0; i < Cookie::Profiler::m_Count; i++)
		{
			Cookie::ProfileResult* result = &Cookie::Profiler::m_Results[i];
			char label[50];
			strcpy(label, result->Name);
			strcat(label, " %.6fms");
			ImGui::Text(label, result->Duration);
		}
		ImGui::End();
		Cookie::Profiler::m_Count = 0;
	}

	void RenderOnUpdate(Cookie::Time Time)
	{
		PROFILE_SCOPE("Render Update");
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
	Cookie::Ref<Cookie::Material> m_InstancedRedMaterial;
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