#pragma once
#include "Viewport.h"
#include "Renderer\MyCamera.h"
#include "events\MouseEvent.h"
#include "events\Event.h"
#include "Renderer\Shader.h"
#include "Renderer\Model.h"

// this class can be used only in imgui context

namespace Referencer {
	class Viewport3D : public Viewport
	{
	private:
		std::string m_name;
		bool m_opened;

		//renderer
		Model m_model;
		Camera m_camera;
		Shader m_shader;

		unsigned int m_fbo, m_texture, m_rbo, m_width, m_height;

		bool m_firstMouse, m_captureMouse;
		float lastX, lastY;

		void renderModel();
		void handleInput(); // uses imgui events so must be in imgui context
		void genBuffers();
		void deleteBuffers();

		
	public:
		Viewport3D(std::string name, bool isOpen);
		~Viewport3D();

		virtual void onUpdate(int offsetX, int offsetY, float instantZoom, float totalZoom) override;
		virtual void onEvent(Event& e) override;
	};
}