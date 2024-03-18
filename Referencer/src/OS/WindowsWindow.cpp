#include "rfpch.h"
#include <Windows.h>
#include "WindowsWindow.h"
#include "events\KeyEvent.h"
#include "events\ApplicationEvents.h"
#include "events\MouseEvent.h"
#include "stb_image.h"

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
			s_GLFWInitialized = true;
		}
		
		//glfwWindowHint(GLFW_DECORATED, GLFW_FALSE // vymysli alternativu alebo si urob resizing sam);
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
		glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
		m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);

		//HWND hWnd = glfwGetWin32Window(m_window);
		//DragAcceptFiles(hWnd, TRUE);

		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, this); // you want this
		glfwSetWindowSizeLimits(m_window, 300, 100, GLFW_DONT_CARE, GLFW_DONT_CARE);
		setVSync(true);

		GLFWimage images[1];
		images[0].pixels = stbi_load("D:/dev/Referencer/Referencer/resources/images/logo.png", &images[0].width, &images[0].height, 0, 4); //rgba channels 
		if (images[0].pixels != nullptr)
			glfwSetWindowIcon(m_window, 1, images);
		stbi_image_free(images[0].pixels);

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
		glfwSetWindowRefreshCallback(m_window, [](GLFWwindow* window)
			{
				//glClear(GL_COLOR_BUFFER_BIT);
				//glfwSwapBuffers(window);
				//glFinish();
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

		glfwSetDropCallback(m_window, [](GLFWwindow* window, int count, const char** paths)
			{
				WindowsWindow& win = *(WindowsWindow*)glfwGetWindowUserPointer(window);

				/*
				HDROP hDrop = (HDROP)glfwGetWin32Window(window);
				if (hDrop != NULL) {
					TCHAR szFile[MAX_PATH];
					UINT i;
					UINT nFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);

					for (i = 0; i < nFiles; i++) {
						if (DragQueryFile(hDrop, i, szFile, MAX_PATH) > 0) {
							// Process each dropped file
							// szFile contains the path of the dropped file
							// Example: Print the file path
							wprintf(L"Dropped file: %s\n", szFile);
							glfwTerminate();
						}
					}

					// Release the dropped files
					DragFinish(hDrop);
				}*/

				DragAndDropEvent e(count, paths);
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
