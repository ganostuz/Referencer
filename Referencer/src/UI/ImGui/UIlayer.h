#pragma once
#include "Layer.h"
#include "events\Event.h"

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include "Application.h"
#include "Viewport.h"
#include "Viewport2D.h"
#include "Viewport3D.h"
#include "GLFW\glfw3.h"

namespace Referencer {

	class UIlayer : public Layer
	{
	private:
		std::vector<Viewport*> m_viewports;
		std::vector<bool> m_selected;
		// meshes and camera rotation matrix
	public:
		UIlayer();
		~UIlayer();

		void onAttach();
		void onDetach();
		void onUpdate();
		void onEvent(Event& e);

		void Begin();
		void End();

	};
}