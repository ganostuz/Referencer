#include "rfpch.h"
#include "WindowsWindow.h"

#include "events\KeyEvent.h"
#include "events\ApplicationEvents.h"
#include "events\MouseEvent.h"

namespace Referencer {

	static bool s_GLFWInitialized = false; // to be implemented error callback

	Window* Window::create(std::string title, unsigned int width, unsigned int height)
	{
		return new WindowsWindow(title, width, height);
	}

	WindowsWindow::WindowsWindow(std::string& title, unsigned int width, unsigned int height)
	{
		init(title, width, height);
	}

	WindowsWindow::~WindowsWindow()
	{
		glfwDestroyWindow(m_window);
	}

	void WindowsWindow::init(std::string& title, unsigned int width, unsigned int height)
	{
		m_title = title;
		m_width = width;
		m_height = height;

		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			if (!success) { std::cout << "glfw was not initialized!" << std::endl; }
			s_GLFWInitialized = true; // todo set glfwErrorCallback
		}
		
		//glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // vymysli alternativu alebo si urob resizing sam
		glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
		m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, this); // you want this
		setVSync(true);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize OpenGL context" << std::endl;
			exit(-1);
		}

		//glfw window callbacks
		glfwSetErrorCallback([](int num, const char* error) 
			{
				std::cout << "[ GLFW ERROR ] [ " << num << " ] " << error << std::endl;
			});

		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) 
			{
			WindowsWindow& win = *(WindowsWindow*)glfwGetWindowUserPointer(window);
			win.m_width = width;
			win.m_height = height;

			WindowResizedEvent e(width, height);
			win.m_eventCallback(e);

			});

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
			{
				WindowsWindow& win = *(WindowsWindow*)glfwGetWindowUserPointer(window);
				WindowCloseEvent e;
				win.m_eventCallback(e);
			});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowsWindow& win = *(WindowsWindow*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent e(key, 0);
					win.m_eventCallback(e);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent e(key);
					win.m_eventCallback(e);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent e(key, 1);
					win.m_eventCallback(e);
					break;
				}
				}
			});
		/* implement this later
		glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int keycode)
			{
				WindowsWindow& win = *(WindowsWindow*)glfwGetWindowUserPointer(window);

				KeyTypedEvent e(keycode);
				win.eventCallback(e);;
			});
		*/
		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowsWindow& win = *(WindowsWindow*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressed e(button);
					win.m_eventCallback(e);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleased e(button);
					win.m_eventCallback(e);
					break;
				}
				}
			});

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowsWindow& win = *(WindowsWindow*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent e((float)xOffset, (float)yOffset);
				win.m_eventCallback(e);
			});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowsWindow& win = *(WindowsWindow*)glfwGetWindowUserPointer(window);

				MouseMovedEvent e((float)xPos, (float)yPos);
				win.m_eventCallback(e);
			});
		glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int character) 
			{
			WindowsWindow& win = *(WindowsWindow*)glfwGetWindowUserPointer(window);

			KeyTypedEvent e(character);
			win.m_eventCallback(e);
			});
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
