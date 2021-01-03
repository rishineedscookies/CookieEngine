#include "ckpch.h"
#include "Renderer.h"

#include "Renderer2D.h"
#include "Cookie/Renderer/Light.h"
#include "Cookie/Asset/Mesh.h"
#include <mathfu/glsl_mappings.h>
#include <mathfu/matrix.h>


namespace Cookie {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;
	Renderer::MeshInstance* Renderer::m_MeshInstances = NULL;
	uint32_t Renderer::m_NumInstances = 0;

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::OnFrameBufferResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera, PointLight* pointLight, DirectionalLight* directionalLight)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		m_SceneData->ViewPosition = camera.GetPosition();
		m_SceneData->PointLightPosition = pointLight->Position;
		m_SceneData->PointLightDiffuse = pointLight->Diffuse;
		m_SceneData->DirectionalLightDirection = directionalLight->Direction;
		m_SceneData->DirectionalLightDiffuse = directionalLight->Diffuse;
		m_MeshInstances = new MeshInstance[20];
	}

	void Renderer::EndScene()
	{	
		for (uint32_t i = 0; i < m_NumInstances; i++)
		{
			MeshInstance* inst = &m_MeshInstances[i];
			inst->Mat->Bind();
			Shader* shader = &*inst->Mat->GetShader();
			shader->Bind();
			UploadSceneUniforms(shader);
			inst->Mat->UploadUniformData();
			inst->ModelMatrices->Reduce(inst->NumInstances);
			VertexBuffer* instanceVertexBuffer = VertexBuffer::Create(inst->ModelMatrices->GetData(), inst->NumInstances * 64);
			BufferLayout layout = BufferLayout({
				{ ShaderDataType::Float4, "a_MVP", 0, false, 1},
				{ ShaderDataType::Float4, "a_MVP", 16, false, 1},
				{ ShaderDataType::Float4, "a_MVP", 32, false, 1},
				{ ShaderDataType::Float4, "a_MVP", 48, false, 1},
				{ ShaderDataType::Float4, "a_NormalModel", 0, false, 1},
				{ ShaderDataType::Float4, "a_NormalModel", 16, false, 1},
				{ ShaderDataType::Float4, "a_NormalModel", 32, false, 1},
				{ ShaderDataType::Float4, "a_NormalModel", 48, false, 1},

			}, 3);
			instanceVertexBuffer->SetLayout(layout);

			VertexArray* vao = VertexArray::Create();
			vao->Bind();
			vao->SetIndexBuffer(inst->Instance->EBO);
			vao->AddVertexBuffer(inst->Instance->VBO);
			vao->AddVertexBuffer(instanceVertexBuffer);
			inst->Instance->VAO->AddVertexBuffer(instanceVertexBuffer);
			//inst->Instance->InstancedModel->Bind();
			//glBufferData(GL_ARRAY_BUFFER, 0, inst->ModelMatrices->GetData(), inst->NumInstances * 64);
			//inst->Instance->InstancedModel->SetData(inst->ModelMatrices->GetData(), inst->NumInstances * sizeof(mathfu::mat4));
			//inst->Instance->VAO->Bind();

			RenderCommand::DrawIndexedInstanced(inst->Instance->VAO, inst->Instance->NumIndices, inst->NumInstances);
			
			inst->Instance->VAO->Unbind();
			delete instanceVertexBuffer;
			delete vao;
		}
		delete[] m_MeshInstances;
		m_NumInstances = 0;
	}

	void Renderer::Submit(const Ref<Shader> shader, const VertexArray* vertexArray, const mathfu::mat4& transform)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		shader->UploadUniformMat4("u_Transform", transform);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::UploadSceneUniforms(Shader* shader)
	{
		shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		shader->UploadUniformFloat3("u_ViewPos", m_SceneData->ViewPosition);
		shader->UploadUniformFloat3("u_PointLightPos", m_SceneData->PointLightPosition);
		shader->UploadUniformFloat3("u_PointLightDiffuse", m_SceneData->PointLightDiffuse);
		shader->UploadUniformFloat3("u_DirectionalLightDirection", m_SceneData->DirectionalLightDirection);
		shader->UploadUniformFloat3("u_DirectionalLightDiffuse", m_SceneData->DirectionalLightDiffuse);
	}

	Renderer::MeshInstance* Renderer::GetMeshInstance(struct Mesh* mesh, class Material* material)
	{
		for (uint32_t i = 0; i < m_NumInstances; i++)
		{
			MeshInstance* inst = &m_MeshInstances[i];
			if (inst && inst->Mat == material && inst->Instance == mesh)
			{
				return inst;
			}
		}
		MeshInstance* inst = &m_MeshInstances[m_NumInstances];
		inst->Instance = mesh;
		inst->Mat = material;
		m_NumInstances++;
		return inst;
	}

	void Renderer::DrawModel(struct Model* model, class Material* material, const mathfu::mat4* transform)
	{
		for (size_t i = 0; i < model->NumMeshes; i++)
		{
			DrawMesh(&(*model->Meshes)[i], material, transform);
		}
	}

	void Renderer::DrawMesh(struct Mesh* mesh, class Material* material, const mathfu::mat4* transform)
	{
		if (mesh->bInstanced)
		{
			SubmitMeshInstance(mesh, material, transform);
			return;
		}
		mathfu::mat4 normalMat = transform->Inverse().Transpose();
		material->GetShader()->Bind();
		UploadSceneUniforms(&*material->GetShader());
		material->GetShader()->UploadUniformMat4("u_Transform", *transform);
		material->GetShader()->UploadUniformMat4("u_NormalTransform", normalMat);
		material->UploadUniformData();
		mesh->VAO->Bind();

		RenderCommand::DrawIndexed(mesh->VAO, mesh->NumIndices);
	}

	void Renderer::SubmitMeshInstance(struct Mesh* mesh, class Material* material, const mathfu::mat4* transform)
	{
		MeshInstance* instance = GetMeshInstance(mesh, material);
		instance->ModelMatrices->Insert(*transform, instance->NumInstances);
		instance->NormalMatrices->Insert(transform->Inverse().Transpose(), instance->NumInstances);
		instance->NumInstances++;
	}

}