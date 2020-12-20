#include "ckpch.h"
#include "OrthographicCamera.h"

namespace Cookie {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(mathfu::mat4::Ortho(left, right, bottom, top, -0.01f, 100.0f)), m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		m_Rotation = mathfu::quat::identity;
	}

	void OrthographicCamera::SetPosition(const mathfu::vec3& position)
	{
		m_Position = position;
		CalculateViewMatrix();
	}

	void OrthographicCamera::SetRotation(const mathfu::quat& rotation)
	{
		m_Rotation = rotation;
		CalculateViewMatrix();
	}

	void OrthographicCamera::SetProjectionMatrix(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = mathfu::mat4::Ortho(left, right, bottom, top, -0.01f, 100.0f);
		//m_ProjectionMatrix = mathfu::mat4::Perspective(60.0f, left / bottom, -0.01f, 100.0f);
	}

	void OrthographicCamera::CalculateViewMatrix()
	{
		mathfu::mat4 translation = mathfu::mat4::FromTranslationVector(m_Position);
		mathfu::mat4 rotation = m_Rotation.ToMatrix4();
		mathfu::mat4 transform = translation * rotation;

		m_ViewMatrix = transform.Inverse();
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}