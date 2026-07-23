#include "rfpch.h"
#include "WindowsWindow.h"
#include "events/KeyEvent.h"
#include "events/ApplicationEvents.h"
#include "events/MouseEvent.h"
#include "ResourcePath.h"
#include "stb_image.h"
#include <cstdlib>

namespace Referencer {

	static bool s_GLFWInitialized = false; // to be implemented error callback

	Window* Window::create(std::string title, unsigned int width, unsigned int height)
	{
		return new GlfwWindow(title, width, height);
	}

	GlfwWindow::GlfwWindow(std::string& title, unsigned int width, unsigned int height)
		: m_window(nullptr), m_width(0), m_height(0), m_VSync(false), m_supportsImGuiViewports(true)
	{
		init(title, width, height);
	}

	GlfwWindow::~GlfwWindow()
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		s_GLFWInitialized = false;
	}

	void GlfwWindow::init(std::string& title, unsigned int width, unsigned int height)
	{
		m_title = title;
		m_width = width;
		m_height = height;

		if (!s_GLFWInitialized)
		{
			glfwSetErrorCallback([](int errorCode, const char* description)
				{
					std::cerr << "[GLFW ERROR] [" << errorCode << "] " << description << std::endl;
				});
			int success = GLFW_FALSE;
#if defined(__linux__)
			if (std::getenv("DISPLAY"))
			{
				glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
				success = glfwInit();
			}

			if (!success)
			{
				glfwInitHint(GLFW_PLATFORM, GLFW_ANY_PLATFORM);
				success = glfwInit();
				m_supportsImGuiViewports = false;
			}
#else
			success = glfwInit();
#endif
			if (!success)
				throw std::runtime_error("GLFW could not be initialized");
			s_GLFWInitialized = true;
		}
		
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
		glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
		m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
		if (!m_window)
			throw std::runtime_error("GLFW could not create the application window");

		//HWND hWnd = glfwGetWin32Window(m_window);
		//DragAcceptFiles(hWnd, TRUE);

		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, this); // you want this
		glfwSetWindowSizeLimits(m_window, 300, 100, GLFW_DONT_CARE, GLFW_DONT_CARE);
		setVSync(true);

		if (m_supportsImGuiViewports)
		{
			GLFWimage images[1];
			const std::string iconPath = resourcePath("images/logo.png").string();
			images[0].pixels = stbi_load(iconPath.c_str(), &images[0].width, &images[0].height, 0, 4);
			if (images[0].pixels != nullptr)
				glfwSetWindowIcon(m_window, 1, images);
			stbi_image_free(images[0].pixels);
		}

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize OpenGL context" << std::endl;
			exit(-1);
		}

		//glfw window callbacks
		glfwSetWindowRefreshCallback(m_window, [](GLFWwindow* window)
			{
				//glClear(GL_COLOR_BUFFER_BIT);
				//glfwSwapBuffers(window);
				//glFinish();
			});
		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) 
			{
			GlfwWindow& win = *static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
			win.m_width = width;
			win.m_height = height;

			WindowResizedEvent e(width, height);
			win.m_eventCallback(e);

			});

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
			{
				GlfwWindow& win = *static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
				WindowCloseEvent e;
				win.m_eventCallback(e);
			});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				GlfwWindow& win = *static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));

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
				GlfwWindow& win = *static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));

				KeyTypedEvent e(keycode);
				win.eventCallback(e);;
			});
		*/
		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
			{
				GlfwWindow& win = *static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));

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
				GlfwWindow& win = *static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));

				MouseScrolledEvent e((float)xOffset, (float)yOffset);
				win.m_eventCallback(e);
			});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
			{
				GlfwWindow& win = *static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));

				MouseMovedEvent e((float)xPos, (float)yPos);
				win.m_eventCallback(e);
			});
		glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int character) 
			{
			GlfwWindow& win = *static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));

			KeyTypedEvent e(character);
			win.m_eventCallback(e);
			});

		glfwSetDropCallback(m_window, [](GLFWwindow* window, int count, const char** paths)
			{
				GlfwWindow& win = *static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));

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

	void GlfwWindow::onUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}

	void GlfwWindow::setVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_VSync = enabled;
	}

	bool GlfwWindow::isVSync() const
	{
		return m_VSync;
	}
}
