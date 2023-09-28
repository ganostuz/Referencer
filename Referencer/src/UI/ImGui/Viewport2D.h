#pragma once
#include "Viewport.h"

namespace Referencer {
	class Viewport2D : public Viewport
	{
	private:
		std::string m_name;
		bool m_opened;
	public:
		Viewport2D(std::string name, bool isOpen);

		virtual void onUpdate() override;
		virtual void onEvent(Event& e) override;
	};
}