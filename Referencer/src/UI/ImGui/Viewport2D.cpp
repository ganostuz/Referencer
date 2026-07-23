#include "rfpch.h"
#include "Viewport2D.h"
#include "Viewport.h"
#include <imgui.h>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Utilities.h"

namespace Referencer {

	const float ZOOM_SENSITIVITY = 10.0f;

	void Viewport2D::handleInput()
	{
		setSelected(true);


		ImGuiIO io = ImGui::GetIO();
		if (io.MouseWheel != 0.0f)
		{
			m_zoom = (io.MouseWheel * ZOOM_SENSITIVITY);
		}
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)) || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete)))
			setRunning(false);
		else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_H)))
			setOpened(false);

		if (ImGui::IsKeyDown(ImGuiKey_MouseRight))
			ImGui::OpenPopup("my_select_popup");

		

		//handle scroll & mouse
	}
	void Viewport2D::pan(float x, float y)
	{
		ImGui::SetNextWindowPos(ImVec2{ m_posX + x, m_posY + y });
	}
	void Viewport2D::zoom(float zoom)
	{

	}
	// constructor takes name bool isOpen and sourcem image path
	Viewport2D::Viewport2D(std::string name, bool isOpen, std::string source, int posX, int posY, std::shared_ptr<ImageData> image)
		:Viewport(name, isOpen), m_texture(0), m_posX(posX), m_posY(posY), m_imageSource(source), m_zoom(0.0f), m_image(image), m_flipHorizontal(false), m_flipVertical(false), m_bgColor(0.2f,0.2f,0.2f,0.2f)
	{
		// TODO: popripade mozes nechat nacitany obrazok do pamate
		if (m_image == nullptr)
		{
			m_image = std::make_shared<ImageData>();
			stbi_set_flip_vertically_on_load(false);
			m_image->pixels = stbi_load(source.c_str(), &m_image->width, &m_image->height, NULL, 4); // ak vrate nullptr zrus vytvorenie okna to iste 3D
		}
		m_width = m_image->width;
		m_height = m_image->height;
		// Create a OpenGL texture identifier
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		// Setup filtering parameters for display
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Upload pixels into texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (*m_image).width, (*m_image).height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (*m_image).pixels);
		// above to render
	}
	Viewport2D::Viewport2D(const Viewport2D& other)
		: Viewport(other), m_texture(0), m_posX(other.m_posX + 10), m_posY(other.m_posY + 10), m_imageSource(other.m_imageSource), m_width(other.m_width), m_height(other.m_height), m_zoom(other.m_zoom), m_flipHorizontal(other.m_flipHorizontal), m_flipVertical(other.m_flipVertical), m_bgColor(other.m_bgColor)
	{
		// Create a new ImageData object with a copy of the pixel data
		m_image = std::make_shared<ImageData>(*other.m_image);
		// Initialize OpenGL texture
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		// Setup filtering parameters for display
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Upload pixels into texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_image->width, m_image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image->pixels);
	}
	Viewport2D::~Viewport2D()
	{
		glDeleteTextures(1, &m_texture);
	}
	// function called every frame
	void Viewport2D::onUpdate(int offsetX, int offsetY, float instantZoom, float totalZoom)
	{
		
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings;

		if (offsetX != 0 || offsetY != 0)
		{
			pan(offsetX, offsetY);
		}

		//ImGui::SetNextWindowSize({ m_width / 2, m_height / 2 }, ImGuiCond_FirstUseEver);

		if (m_zoom != 0.0f)
		{
			// prv resiznut podla minuleho framu nasledne prijat dalsi input
			ImGui::SetNextWindowSize({ m_width + m_zoom, m_height + m_zoom });

			m_zoom = 0.0f;
		}

		ImGui::PushStyleColor(ImGuiCol_WindowBg, m_bgColor);
		if (ImGui::Begin((this->getName() + "###" + std::to_string(getID().get())).c_str(), &isRunning(), windowFlags))
		{
			
			m_width = ImGui::GetWindowSize().x;
			m_height = ImGui::GetWindowSize().y;

			m_posX = ImGui::GetWindowPos().x;
			m_posY = ImGui::GetWindowPos().y;

			if (ImGui::IsWindowHovered())
				handleInput();
			else
				setSelected(false);
			// will go to render
			
			ImVec2 uv1(m_flipHorizontal == true ? 1.0f : 0.0f, m_flipVertical == true ? 1.0f : 0.0f);
			ImVec2 uv2(m_flipHorizontal == true ? 0.0f : 1.0f, m_flipVertical == true ? 0.0f : 1.0f);

			ImVec2 windowSize = ImGui::GetWindowSize();
			ImVec2 imageSize(256, 256); // Example size, adjust based on your image
			ImVec2 imagePos = ImVec2(
				(windowSize.x - imageSize.x) / 2,
				(windowSize.y - imageSize.y) / 2
			);
			ImGuiIO& io = ImGui::GetIO();
			// Render the image
			ImGui::SetCursorPos(imagePos);

			ImGui::Image((void*)(intptr_t)m_texture, imageSize, uv1, uv2);
			

			ImRect rc = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
			ImVec2 mouseUVCoord = (io.MousePos - rc.Min) / rc.GetSize();

			if (io.KeyCtrl &&  mouseUVCoord.x >= 0.f && mouseUVCoord.y >= 0.f && mouseUVCoord.x <= 1.f && mouseUVCoord.y <= 1.f)
			{

				Utilities::inspect(m_image->width, m_image->height, m_image->pixels, mouseUVCoord, ImVec2(256,256));
			}
			if (ImGui::BeginPopup("my_select_popup"))
			{
				ImGui::MenuItem("Opened", "Del", &isRunning());
				ImGui::MenuItem("Visible", "H", &isOpened());
				
				if (ImGui::BeginMenu("transform"))
				{
					ImGui::MenuItem("Flip horizontal", "", &m_flipHorizontal);
					ImGui::MenuItem("Flip vertical", "", &m_flipVertical);
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("edit colors"))
				{
					ImGui::ColorPicker3("Background Color", (float*)&m_bgColor, ImGuiColorEditFlags_Float);

					ImGui::EndMenu();
				}
				ImGui::EndPopup();
			}

		}
		ImGui::End();
		ImGui::PopStyleColor();

		

		

	}
	//event handler
	void Viewport2D::onEvent(Event& e)
	{
	}

	int Viewport2D::getType()
	{
		return 0;
	}

	Viewport* Viewport2D::clone()
	{
		return new Viewport2D(*this);
	}
	void Viewport2D::SerializeImage(std::ostream& os) const {
		// Write the width and height

		os.write(reinterpret_cast<char*>(&this->m_image->width), sizeof(this->m_image->width));
		os.write(reinterpret_cast<char*>(&this->m_image->height), sizeof(this->m_image->height));

		// Write the pixel data // unsigned?
		os.write(reinterpret_cast<char*>(this->m_image->pixels), this->m_image->width * this->m_image->height * 4); // Assuming  4 bytes per pixel (RGBA)
	}

}