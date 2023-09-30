#pragma once

#include "Window.h"

#include "glad\glad.h"
#include <GLFW/glfw3.h>

namespace Referencer {

	class WindowsWindow : public Window
	{
	private:
		virtual void init(std::string& title, unsigned int width, unsigned int height);// daj window properties prec z window

		GLFWwindow* m_window;
		std::string m_title;
		unsigned int m_width, m_height;
		bool m_VSync;

		//using EventCallbackFn std::function<bool(Event e)>; // asi to este niekde prehodim

		EventCallbackFn m_eventCallback;
	public:
		WindowsWindow(std::string& title, unsigned int width, unsigned int height);
		virtual ~WindowsWindow();

		void onUpdate() override;

		inline unsigned int getWidth() const override { return m_width; }
		inline unsigned int getHeight() const override { return m_height; }

		// Window attributes
		inline void setEventCallback(const EventCallbackFn& callback) override { m_eventCallback = callback; }
		void setVSync(bool enabled) override;
		bool isVSync() const override;

		inline virtual void* getNativeWindow() const { return m_window; }
	};

}