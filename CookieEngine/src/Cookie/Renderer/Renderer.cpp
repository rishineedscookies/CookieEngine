#include "ckpch.h"
#include "Renderer.h"

#include "Renderer2D.h"
#include "Cookie/Renderer/Light.h"
#include "Cookie/Asset/Mesh.h"
#include <mathfu/glsl_mappings.h>
#include <mathfu/matrix.h>


namespace Cookie {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;
	Renderer::InstancedMesh* Renderer::m_MeshInstances = NULL;
	uint32_t Renderer::m_NumMats = 0;

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
		//delete[] m_MeshInstances;
		//m_MeshInstances = new InstancedMesh[20];
		//m_NumMats = 0;
	}

	void Renderer::EndScene()
	{
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

	Renderer::InstancedMesh* Renderer::GetMeshInstance(struct Mesh* mesh, class Material* material)
	{
		return NULL;
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
		/*MaterialInstance* instance = GetMaterialInstance(material);
		CK_CORE_ASSERT(instance->VertexCount + mesh->NumVertices > 60000, "Trying to draw too many vertices in one material instance");
		CK_CORE_ASSERT(instance->IndexCount + mesh->NumIndices > instance->MaxIndices, "Trying to draw too many indices in one material instance");
		memcpy(instance->VertexBufferPtr, mesh->Vertices->GetData(), sizeof(mesh->Vertices->GetData()));
		memcpy(instance->IndexBufferPtr, mesh->Indices->GetData(), sizeof(mesh->Indices->GetData()));
		instance->VertexBufferPtr += mesh->NumVertices;
		instance->IndexBufferPtr += mesh->NumIndices;
		m_NumMats++;*/
		mathfu::mat4 normalMat = transform->Inverse().Transpose();
		material->GetShader()->Bind();
		UploadSceneUniforms(&*material->GetShader());
		material->GetShader()->UploadUniformMat4("u_Transform", *transform);
		material->GetShader()->UploadUniformMat4("u_NormalTransform", normalMat);
		material->UploadUniformData();
		mesh->VAO->Bind();

		RenderCommand::DrawIndexed(mesh->VAO, mesh->NumIndices);
	}

	void Renderer::SubmitInstanceMesh(struct Mesh* mesh, class Material* material, const mathfu::mat4* transform)
	{
		InstancedMesh* instance = GetMeshInstance(mesh, material);
		
	}

}