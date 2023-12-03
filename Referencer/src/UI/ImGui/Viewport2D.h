#pragma once
#include "Viewport.h"

// this class can be used only in imgui context

namespace Referencer {
	class Viewport2D : public Viewport
	{
	private:
		float m_width, m_height;
		float m_posX, m_posY;
		float m_ratio; // width to heigh
		unsigned int m_texture;

		void handleInput();
		void pan(float x, float y);
		void zoom(float zoom);
	public:
		Viewport2D(std::string name, bool isOpen, std::string source, int posX = 10, int posY = 10);

		virtual void onUpdate(int offsetX, int offsetY, float instantZoom, float totalZoom) override;
		virtual void onEvent(Event& e) override;
	};
}