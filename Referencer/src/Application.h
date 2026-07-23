#pragma once

#include "Window.h"
#include "events/ApplicationEvents.h"
#include "LayerStack.h"
#include "Settings.h"


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

		void serialize();
		void deserialize();
		inline Window& getWindow() { return *m_window; }
		inline static Application& getApplication() { return *s_instance; }
		inline SettingsHolder& getSettings() { return m_settings.getSettings(); }

	private:
		Settings m_settings;
		std::unique_ptr<Window> m_window;
		LayerStack m_layerStack;
		bool m_running;
		static Application* s_instance;

		bool onWindowClose(WindowCloseEvent& e);
	};
}
