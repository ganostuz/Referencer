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

		//renderer
		Model m_model;
		Camera m_camera;
		Shader m_shader;

		unsigned int m_fbo, m_texture, m_rbo, m_width, m_height;
		bool m_firstMouse, m_first_time, m_hasTexture, m_showSettings; // one time usage
		glm::vec2 m_lastMouse;
		//light, color, material settings
		float m_lightStrength, m_scale, m_interpolation;
		float m_objectColor[3], m_lightColor[3], m_lightPos[3], m_translate[3];
		

		void handleInput(); // uses imgui events so must be in imgui context
		void genBuffers();
		void deleteBuffers();
		void renderModel();
		void renderImGuiModelSpecs();
		void renderImGuiModel();
		void computeLightPos(float distance, glm::vec2 deltaAngleRadians);

	public:
		Viewport3D(std::string name, bool isOpen);
		~Viewport3D();

		virtual void onUpdate(int offsetX, int offsetY, float instantZoom, float totalZoom) override;
		virtual void onEvent(Event& e) override;
	};
}