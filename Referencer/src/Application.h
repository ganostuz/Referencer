#pragma once

#include "Window.h"
#include "events\ApplicationEvents.h"

namespace Referencer {
	class Application
	{
	public:
		Application();
		~Application();

		void onEvent(Event& e);
		void run();

		bool onWindowClose(WindowCloseEvent& e);
	private:

		std::unique_ptr<Window> m_window;
		bool m_running;
	};
}
