#pragma once

#include <mathfu/matrix.h>
#include <mathfu/vector.h>
#include <mathfu/glsl_mappings.h>

namespace Cookie {

	class OrthographicCamera
	{
	public: 
		OrthographicCamera(float left, float right, float bottom, float top);

		const mathfu::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const mathfu::vec3& position);

		const mathfu::quat& GetRotation() const { return m_Rotation; }
		void SetRotation(const mathfu::quat& rotation);

		const mathfu::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const mathfu::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const mathfu::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		void CalculateViewMatrix();

	private:
		mathfu::mat4 m_ProjectionMatrix;
		mathfu::mat4 m_ViewMatrix;
		mathfu::mat4 m_ViewProjectionMatrix;

		mathfu::vec3 m_Position;
		mathfu::quat m_Rotation;
	};

}