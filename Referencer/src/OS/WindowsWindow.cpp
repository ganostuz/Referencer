#include "rfpch.h"
#include "WindowsWindow.h"

#include "events\KeyEvent.h"
#include "events\ApplicationEvents.h"
#include "events\MouseEvent.h"

namespace Referencer {

	static bool s_GLFWInitialized = false; // to be implemented error callback

	Window* Window::create(const WindowProperties& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProperties& props)
	{
		init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		glfwDestroyWindow(m_window);
	}

	void WindowsWindow::init(const WindowProperties& props)
	{
		m_title = props.title;
		m_width = props.width;
		m_height = props.height;

		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			if (!success) { std::cout << "glfw was not initialized!" << std::endl; }
			s_GLFWInitialized = true; // todo set glfwErrorCallback
		}

		m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_window);
		//glfwSetWindowUserPointer(m_window, this); // id ont't know if i want this
		setVSync(true);
	}

	void WindowsWindow::onUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}

	void WindowsWindow::setVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_VSync = enabled;
	}

	bool WindowsWindow::isVSync() const
	{
		return m_VSync;
	}


}
