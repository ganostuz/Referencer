#pragma once

#include "Window.h"

namespace Referencer {
	class Application
	{
	public:
		Application();
		~Application();

		void run();

	private:

		std::unique_ptr<Window> m_window;
		bool m_running;
	};
}
