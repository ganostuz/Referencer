#pragma once
#include "Viewport.h"

// this class can be used only in imgui context

namespace Referencer {
	class Viewport2D : public Viewport
	{
	private:
		int width, height;
		float ratio; // width to heigh
		unsigned int texture;
	public:
		Viewport2D(std::string name, bool isOpen, std::string source);

		virtual void onUpdate() override;
		virtual void onEvent(Event& e) override;
	};
}