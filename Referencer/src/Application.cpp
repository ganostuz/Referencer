#include "rfpch.h"
#include "Application.h"


#include "GLFW\glfw3.h"

namespace Referencer {
	Application::Application()
	{
		m_running = true;
		m_window = std::unique_ptr<Window>(Window::create());
		m_window->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1)); // vytvor nejaky define pre toto
	}
	Application::~Application()
	{
	}
	void Application::onEvent(Event& e)
	{
		std::cout << e << std::endl;
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(std::bind(&Application::onWindowClose, this, std::placeholders::_1));

	}
	void Application::run()
	{
		while (m_running)
		{
			m_window->onUpdate();
		}
	}

	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		m_running = false;
		return true;
	}

}

