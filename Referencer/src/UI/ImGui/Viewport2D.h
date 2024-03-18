#pragma once
#include "Viewport.h"
#include <imgui.h>

// this class can be used only in imgui context

namespace Referencer {
	struct ImageData {
		int width;
		int height;
		unsigned char* pixels;
	};
	class Viewport2D : public Viewport
	{
	private:
		// vytvor struct, kt. budes mat tu a nasledne iba transformujes nic viac nemenis samotny struct
		std::shared_ptr<ImageData> m_image;
		float m_width, m_height;
		float m_posX, m_posY;
		float m_zoom; // width to heigh
		unsigned int m_texture;
		std::string m_imageSource;
		bool m_flipVertical, m_flipHorizontal;
		ImVec4 m_bgColor;

		void handleInput();
		void pan(float x, float y);
		void zoom(float zoom);
	public:
		Viewport2D(std::string name, bool isOpen, std::string source, int posX = 10, int posY = 10, std::shared_ptr<ImageData> image = nullptr);
		Viewport2D(const Viewport2D& other);
		~Viewport2D();

		void SerializeImage(std::ostream& os) const;
		virtual void onUpdate(int offsetX, int offsetY, float instantZoom, float totalZoom) override;
		virtual void onEvent(Event& e) override;
		virtual int getType() override;
		virtual Viewport* clone() override;
	};
}