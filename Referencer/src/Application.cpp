#include "rfpch.h"

#include "Application.h"
#include "UI\ImGui\UIlayer.h"
#include "GLFW\glfw3.h"
#include "glad\glad.h"
#include "glm/common.hpp"
#include "assimp\BaseImporter.h"
#include "TestLayer.h"
#include "stb_image.h"

namespace Referencer {

	Application* Application::s_instance = nullptr;
	Application::Application()
	{
		s_instance = this;
		m_running = true;
		m_window = std::unique_ptr<Window>(Window::create());
		m_window->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1)); // vytvor nejaky define pre toto
		//m_window->glfwSetDropCallback(window, drop_callback);

		glEnable(GL_DEPTH_TEST);
		stbi_set_flip_vertically_on_load(true);

		//pushLayer(new TestLayer());
		pushLayer(new UIlayer());

	}
	Application::~Application()
	{
	}
	void Application::onEvent(Event& e)
	{
		std::cout << "[APP] " << e << std::endl;
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(std::bind(&Application::onWindowClose, this, std::placeholders::_1));
		dispatcher.dispatch<WindowResizedEvent>([](WindowResizedEvent& e) -> bool
		{
			glViewport(0, 0, e.getWidth(), e.getHeight());

			return false;// for now
		}); // window resize, moved and closed events will be handled primarly by application and handled set to false primarly beacause ImGui needs to process it

		for (auto i = m_layerStack.begin(); i != m_layerStack.end(); i++)
		{
			if (e.isHandled())
				break;
			(*i)->onEvent(e);
		}


	}
	void Application::run()
	{

		while (m_running)
		{
			glClearColor(.2f,.2f,.2f,1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

