#pragma once

#include "Window.h"
#include "events\ApplicationEvents.h"
#include "LayerStack.h"


namespace Referencer {

	class Application
	{
	public:
		Application();
		~Application();

		void onEvent(Event& e);
		void run();

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);

		inline Window& getWindow() { return *m_window; }
		inline static Application& getApplication() { return *s_instance; }

	private:
		LayerStack m_layerStack;
		std::unique_ptr<Window> m_window;
		bool m_running;
		static Application* s_instance;

		bool onWindowClose(WindowCloseEvent& e);
	};
}
