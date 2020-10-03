#pragma once

#include "Cookie/Core/Layer.h"
#include "Cookie/Events/KeyEvent.h"
#include "Cookie/Events/MouseEvent.h"
#include "Cookie/Events/ApplicationEvent.h"

namespace Cookie {

	class COOKIE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender();

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};
}