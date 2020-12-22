#include "ckpch.h"
#include "Renderer.h"

#include "Renderer2D.h"
#include "Cookie/Renderer/Light.h"
#include "Cookie/Asset/Mesh.h"
#include <mathfu/glsl_mappings.h>
#include <mathfu/matrix.h>


namespace Cookie {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

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

	void Renderer::DrawModel(const struct Model* model, class Material* material, const mathfu::mat4* transform)
	{
		for (size_t i = 0; i < model->NumMeshes; i++)
		{
			DrawMesh(const_cast<const Mesh*>(&(*model->Meshes)[i]), material, transform);
		}
	}

	void Renderer::DrawMesh(const struct Mesh* mesh, class Material* material, const mathfu::mat4* transform)
	{
		material->GetShader()->Bind();
		UploadSceneUniforms(&*material->GetShader());
		material->GetShader()->UploadUniformMat4("u_Transform", *transform);
		material->UploadUniformData();
		mesh->VAO->Bind();

		RenderCommand::DrawIndexed(mesh->VAO, mesh->NumIndices);
	}

}