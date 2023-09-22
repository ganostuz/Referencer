#pragma once
#include "Layer.h"

namespace Referencer {

	class ExampleLayer : public Layer
	{
		ExampleLayer()
			: Layer("Example Layer") {}


		virtual void onUpdate() override {
			std::cout << "[example layer] updated!" << std::endl;
		}
		virtual void onEvent(Event& e) {
			std::cout << e << std::endl;
		}
	};

}
