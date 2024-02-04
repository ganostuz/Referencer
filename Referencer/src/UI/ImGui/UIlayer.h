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
		bool m_showMenu, m_wasCopyed;

		char m_renameBuffer[30];
		Viewport* m_renameViewportPointer;
		
		
		

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

		bool handleDrops(DragAndDropEvent& e);

		

	};
}