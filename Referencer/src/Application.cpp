#include "rfpch.h"
#include "Application.h"

#include "glad\glad.h"
#include "GLFW\glfw3.h"

namespace Referencer {

	class ExampleLayer : public Layer
	{ 
	public:
		ExampleLayer()
			: Layer("[example layer]")
		{
		}


		virtual void onUpdate() override {
			std::cout << "[example layer] updated!" << std::endl;
		}
		virtual void onEvent(Event& e) {
			std::cout << this->getName() << e << std::endl;
		}
	};
	Application::Application()
	{

		m_running = true;
		m_window = std::unique_ptr<Window>(Window::create());
		m_window->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1)); // vytvor nejaky define pre toto

		pushLayer(new ExampleLayer());
	}
	Application::~Application()
	{
	}
	void Application::onEvent(Event& e)
	{
		std::cout << "[APP] " << e << std::endl;
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(std::bind(&Application::onWindowClose, this, std::placeholders::_1));

		for (auto i = m_layerStack.begin(); i != m_layerStack.end(); i++)
		{
			(*i)->onEvent(e);
			if (e.isHandled())
				break;
		}


	}
	void Application::run()
	{
		while (m_running)
		{

			for (Layer* layer : m_layerStack)
				layer->onUpdate();

			m_window->onUpdate();
		}
	}

	void Application::pushLayer(Layer* layer)
	{
		m_layerStack.pushLayer(layer);
		layer->onAttach();
	}

	void Application::pushOverlay(Layer* layer)
	{
		m_layerStack.pushOverlay(layer);
		layer->onAttach();
	}

	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		m_running = false;
		return true;
	}

}

