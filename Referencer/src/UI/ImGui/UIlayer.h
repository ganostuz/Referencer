#pragma once
#include "Layer.h"
#include "events\Event.h"

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include "Application.h"
#include "Viewport.h"
#include "Viewport2D.h"
#include "Viewport3D.h"



namespace Referencer {

	class UIlayer : public Layer
	{
	private:
		std::vector<Viewport*> m_viewports;
		float m_zoom, m_instantZoom;
		int m_offsetX, m_offsetY, m_instantOffsetX, m_instantOffsetY, m_viewportIndex;
		bool m_showMenu; 
		

	public:
		UIlayer();
		~UIlayer();

		void onAttach();
		void onDetach();
		void onUpdate();
		void onEvent(Event& e);

	private:

		void Begin();
		void End();
		void Render();
		void UpdateViewports();
		std::string saveFileDialog(const char* filter);
		std::string loadFileDialog(const char* filter);

		void RenderMainMenu();
		void RenderMenu();
		void RenderLayerManager();
		void RenderViewports();

		static void Drop_callback(GLFWwindow* window, int count, const char** paths) {
			for (int i = 0; i < count; i++) {
				// paths[i] contains the full path of the dropped file
				// Use this path as needed in your application
				//m_viewports.push_back(new Viewport2D(std::to_string(123) + std::to_string(m_viewportIndex), true, paths[i]));
				//m_viewportIndex++;
				const char* imagePath = paths[i];
				// handle it
				std::cout << imagePath << std::endl;
				
			}
		}

		

	};
}