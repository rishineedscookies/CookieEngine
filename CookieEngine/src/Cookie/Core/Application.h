#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Cookie/Events/Event.h"
#include "Cookie/Events/ApplicationEvent.h"
#include "Cookie/ImGui/ImGuiLayer.h"

#include "Cookie/Renderer/Shader.h"
#include "Cookie/Renderer/Buffer.h"
#include "Cookie/Renderer/VertexArray.h"
#include "Cookie/Renderer/OrthographicCamera.h"

#include "Cookie/Core/Time.h"

namespace Cookie {

	class COOKIE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		Time m_Time;

	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}

