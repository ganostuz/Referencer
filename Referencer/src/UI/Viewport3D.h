#pragma once
#include "Layer.h"
#include "events\Event.h"
#include "ImGui\imgui_impl_opengl3.h"
#include "Application.h"
#include "GLFW\glfw3.h"

namespace Referencer {

	class Viewport3D : public Layer
	{
	private:
		float m_time = 0.0f;
		// meshes and camera rotation matrix
	public:
		Viewport3D();
		~Viewport3D();

		void onAttach() ;
		void onDetach() ;
		void onUpdate();
		void onEvent(Event& e);

	};
}